#include "nj_api.h"

#include "Jit.hpp"
#include "compile/Compilation.hpp"
#include "compile/CompilationTypes.hpp"
#include "compile/Method.hpp"
#include "compile/SymbolReferenceTable.hpp"
#include "control/CompileMethod.hpp"
#include "env/jittypes.h"
#include "il/Block.hpp"
#include "il/DataTypes.hpp"
#include "il/ILOpCodes.hpp"
#include "il/Node.hpp"
#include "il/Node_inlines.hpp"
#include "il/TreeTop.hpp"
#include "il/TreeTop_inlines.hpp"
#include "ilgen/IlGeneratorMethodDetails_inlines.hpp"
#include "ilgen/IlInjector.hpp"
#include "ilgen/TypeDictionary.hpp"
#include "infra/Cfg.hpp"

#include <mutex>
#include <string>

namespace nj {

class FunctionBuilder;

class Context {
public:
  Context() {}
  FunctionBuilder *newFunctionBuilder(const char *name, JIT_Type return_type,
                                      JIT_ILBuilder ilbuilder, void *userdata);

private:
  TR::TypeDictionary types;
};

static std::mutex s_jitlock;
static volatile int s_ctxcount;

class SimpleILInjector : public TR::IlInjector {
public:
  SimpleILInjector(TR::TypeDictionary *types, FunctionBuilder *function_builder)
      : TR::IlInjector(types), types_(types),
        function_builder_(function_builder) {}

  bool injectIL() override; /* override */

  TR::TypeDictionary *types_;
  FunctionBuilder *function_builder_;
  std::map<std::string, TR::SymbolReference *>
      symRefMap_; // mapping of string names to symrefs
  std::map<std::string, int>
      blockMap_; // mapping of string names to basic block numbers
  std::map<std::string, TR::Node *> nodeMap_;
};

class FunctionBuilder {
public:
  TR::TypeDictionary *types_;
  SimpleILInjector ilgenerator_;
  char name_[128];
  TR::DataTypes return_type_;
  std::vector<TR::DataTypes> args_;
  JIT_ILBuilder ilbuilder_;
  void *userdata_;
  char file_[30];
  char line_[30];

  FunctionBuilder(TR::TypeDictionary *types, const char *name,
                  JIT_Type return_type, int argc, JIT_FunctionParameter *args,
                  JIT_ILBuilder ilbuilder, void *userdata)
      : types_(types), ilgenerator_(types, this),
        return_type_((TR::DataTypes)return_type), ilbuilder_(ilbuilder),
        userdata_(userdata) {
    strcpy(file_, "file");
    strcpy(line_, "line");
    strncpy(name_, name, sizeof name_);
    name_[sizeof name_ - 1] = 0;
    for (int i = 0; i < argc; i++) {
      args_.push_back((TR::DataTypes)args[i].type);
    }
  }

  void *compile() {
    // construct a `TR::ResolvedMethod` instance from the IL generator and use
    // to compile the method
    auto argIlTypes = std::vector<TR::IlType *>(args_.size());
    for (int i = 0; i < args_.size(); i++) {
      argIlTypes[i] = types_->PrimitiveType(args_[i]);
    }
    TR::ResolvedMethod resolvedMethod(
        file_, line_, name_, argIlTypes.size(), argIlTypes.data(),
        types_->PrimitiveType(return_type_), 0, &ilgenerator_);
    TR::IlGeneratorMethodDetails methodDetails(&resolvedMethod);
    int32_t rc = 0;
    uint8_t *entry_point =
        compileMethodFromDetails(NULL, methodDetails, warm, rc);
    if (rc == 0)
      return entry_point;
    return nullptr;
  }
};

FunctionBuilder *Context::newFunctionBuilder(const char *name,
                                             JIT_Type return_type,
                                             JIT_ILBuilder ilbuilder,
                                             void *userdata) {
  FunctionBuilder *function_builder = new FunctionBuilder(
      &types, name, return_type, 0, NULL, ilbuilder, userdata);
  return function_builder;
}

static inline JIT_ContextRef wrap_context(Context *p) {
  return reinterpret_cast<JIT_ContextRef>(p);
}

static inline Context *unwrap_context(JIT_ContextRef p) {
  return reinterpret_cast<Context *>(p);
}

static inline JIT_ILInjectorRef wrap_ilinjector(SimpleILInjector *p) {
  return reinterpret_cast<JIT_ILInjectorRef>(p);
}

static inline SimpleILInjector *unwrap_ilinjector(JIT_ILInjectorRef p) {
  return reinterpret_cast<SimpleILInjector *>(p);
}

static inline JIT_FunctionBuilderRef wrap_function_builder(FunctionBuilder *p) {
  return reinterpret_cast<JIT_FunctionBuilderRef>(p);
}

static inline FunctionBuilder *
unwrap_function_builder(JIT_FunctionBuilderRef p) {
  return reinterpret_cast<FunctionBuilder *>(p);
}

static inline JIT_BlockRef wrap_block(TR::Block *p) {
  return reinterpret_cast<JIT_BlockRef>(p);
}

static inline TR::Block *unwrap_block(JIT_BlockRef p) {
  return reinterpret_cast<TR::Block *>(p);
}

static inline JIT_NodeRef wrap_node(TR::Node *p) {
  return reinterpret_cast<JIT_NodeRef>(p);
}

static inline TR::Node *unwrap_node(JIT_NodeRef p) {
  return reinterpret_cast<TR::Node *>(p);
}

static inline JIT_TreeTopRef wrap_treetop(TR::TreeTop *p) {
  return reinterpret_cast<JIT_TreeTopRef>(p);
}

static inline TR::TreeTop *unwrap_node(JIT_TreeTopRef p) {
  return reinterpret_cast<TR::TreeTop *>(p);
}

static inline JIT_CFGNodeRef wrap_cfgnode(TR::CFGNode *p) {
  return reinterpret_cast<JIT_CFGNodeRef>(p);
}

static inline TR::CFGNode *unwrap_cfgnode(JIT_CFGNodeRef p) {
  return reinterpret_cast<TR::CFGNode *>(p);
}

static inline JIT_SymbolRef wrap_symbolref(TR::SymbolReference *p) {
  return reinterpret_cast<JIT_SymbolRef>(p);
}

static inline TR::SymbolReference *unwrap_symbolref(JIT_SymbolRef p) {
  return reinterpret_cast<TR::SymbolReference *>(p);
}

bool SimpleILInjector::injectIL() {
  return (function_builder_->ilbuilder_(wrap_ilinjector(this),
                                        function_builder_->userdata_));
}

} // namespace nj

using namespace nj;

extern "C" {

JIT_ContextRef JIT_CreateContext() {
  {
    std::lock_guard<std::mutex> g(s_jitlock);
    if (s_ctxcount == 0)
      if (!initializeJit())
        return nullptr;
    s_ctxcount++;
  }

  Context *context = new Context();

  return wrap_context(context);
}

void JIT_DestroyContext(JIT_ContextRef ctx) {
  Context *context = unwrap_context(ctx);
  if (!context)
    return;
  {
    delete context;
    std::lock_guard<std::mutex> g(s_jitlock);
    s_ctxcount--;
    if (s_ctxcount == 0)
      shutdownJit();
  }
}

JIT_FunctionBuilderRef
JIT_CreateFunctionBuilder(JIT_ContextRef ctx, const char *name,
                          enum JIT_Type return_type, int param_count,
                          JIT_FunctionParameter *parameters,
                          JIT_ILBuilder ilbuilder, void *userdata) {
  Context *context = unwrap_context(ctx);
  return wrap_function_builder(
      context->newFunctionBuilder(name, return_type, ilbuilder, userdata));
}

void JIT_DestroyFunctionBuilder(JIT_FunctionBuilderRef fb) {
  FunctionBuilder *function_builder = unwrap_function_builder(fb);
  delete function_builder;
}

void *JIT_Compile(JIT_FunctionBuilderRef fb) {
  FunctionBuilder *function_builder = unwrap_function_builder(fb);
  return function_builder->compile();
}

void JIT_CreateBlocks(JIT_ILInjectorRef ilinjector, int32_t num) {
  auto injector = unwrap_ilinjector(ilinjector);
  injector->createBlocks(num);
}

void JIT_SetCurrentBlock(JIT_ILInjectorRef ilinjector, int32_t b) {
  auto injector = unwrap_ilinjector(ilinjector);
  injector->generateToBlock(b);
}

JIT_BlockRef JIT_GetCurrentBlock(JIT_ILInjectorRef ilinjector) {
  auto injector = unwrap_ilinjector(ilinjector);
  return wrap_block(injector->getCurrentBlock());
}

JIT_NodeRef JIT_ConstInt32(int32_t i) { return wrap_node(TR::Node::iconst(i)); }

JIT_NodeRef JIT_CreateNode1C(JIT_NodeOpCode opcode, JIT_NodeRef c1) {
  auto n1 = unwrap_node(c1);
  return wrap_node(TR::Node::create((TR::ILOpCodes)opcode, 1, n1));
}

JIT_TreeTopRef JIT_GenerateTreeTop(JIT_ILInjectorRef ilinjector,
                                   JIT_NodeRef n) {
  auto injector = unwrap_ilinjector(ilinjector);
  auto node = unwrap_node(n);
  return wrap_treetop(injector->genTreeTop(node));
}

void JIT_CFGAddEdge(JIT_ILInjectorRef ilinjector, JIT_CFGNodeRef from,
                    JIT_CFGNodeRef to) {
  auto injector = unwrap_ilinjector(ilinjector);
  auto node1 = unwrap_cfgnode(from);
  auto node2 = unwrap_cfgnode(to);
  injector->cfg()->addEdge(node1, node2);
}

JIT_CFGNodeRef JIT_BlockAsCFGNode(JIT_BlockRef b) {
  auto block = unwrap_block(b);
  return wrap_cfgnode(static_cast<TR::CFGNode *>(block));
}

JIT_CFGNodeRef JIT_GetCFGEnd(JIT_ILInjectorRef ilinjector) {
  auto injector = unwrap_ilinjector(ilinjector);
  return wrap_cfgnode(injector->cfg()->getEnd());
}

JIT_SymbolRef JIT_CreateTemporary(JIT_ILInjectorRef ilinjector, JIT_Type type) {
  auto injector = unwrap_ilinjector(ilinjector);
  return wrap_symbolref(injector->symRefTab()->createTemporary(
      injector->methodSymbol(), TR::DataType((TR::DataTypes)type)));
}

JIT_SymbolRef JIT_CreateLocalByteArray(JIT_ILInjectorRef ilinjector,
                                       uint32_t size) {
  auto injector = unwrap_ilinjector(ilinjector);
  return wrap_symbolref(injector->symRefTab()->createLocalPrimArray(
      size, injector->methodSymbol(),
      8 /*apparently 8 means Java bytearray! */));
}

JIT_NodeRef JIT_LoadTemporary(JIT_ILInjectorRef ilinjector,
                              JIT_SymbolRef symbol) {
  auto injector = unwrap_ilinjector(ilinjector);
  return wrap_node(injector->loadTemp(unwrap_symbolref(symbol)));
}

void JIT_StoreToTemporary(JIT_ILInjectorRef ilinjector, JIT_SymbolRef symbol,
                          JIT_NodeRef value) {
  auto injector = unwrap_ilinjector(ilinjector);
  return injector->storeToTemp(unwrap_symbolref(symbol), unwrap_node(value));
}

JIT_NodeRef JIT_LoadAddress(JIT_ILInjectorRef ilinjector, JIT_SymbolRef symbol) {
	auto symref = unwrap_symbolref(symbol);
	return wrap_node(TR::Node::createWithSymRef(TR::loadaddr, 0, symref));
}

JIT_NodeRef JIT_ArrayLoad(JIT_ILInjectorRef ilinjector, JIT_NodeRef basenode,
                          JIT_NodeRef indexnode, JIT_Type dt) {
  auto injector = unwrap_ilinjector(ilinjector);
  auto base = unwrap_node(basenode);
  auto index = unwrap_node(indexnode);
  auto type = TR::DataType((TR::DataTypes)dt);
  TR::SymbolReference *symRef =
      injector->symRefTab()->findOrCreateArrayShadowSymbolRef(type, base);
  TR::Node *load = TR::Node::createWithSymRef(
      TR::ILOpCode::indirectLoadOpCode(type), 1, index, 0, symRef);
  return wrap_node(load);
}

void JIT_ArrayStore(JIT_ILInjectorRef ilinjector, JIT_NodeRef basenode,
	JIT_NodeRef indexnode, JIT_NodeRef valuenode) {
	auto injector = unwrap_ilinjector(ilinjector);
	auto base = unwrap_node(basenode);
	auto index = unwrap_node(indexnode);
	auto value = unwrap_node(valuenode);
	auto type = value->getDataType();
	TR::SymbolReference *symRef =
		injector->symRefTab()->findOrCreateArrayShadowSymbolRef(type, base);
	TR::ILOpCodes storeOp = injector->comp()->il.opCodeForIndirectArrayStore(type);
	TR::Node *store = TR::Node::createWithSymRef(
		storeOp, 2, index, value, 0, symRef);
	injector->genTreeTop(store);
}

} // extern "C"
