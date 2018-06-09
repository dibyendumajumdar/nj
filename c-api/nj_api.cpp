#include "nj_api.h"

#include "Jit.hpp"
#include "compile/Compilation.hpp"
#include "compile/CompilationTypes.hpp"
#include "compile/Method.hpp"
#include "control/CompileMethod.hpp"
#include "env/jittypes.h"
#include "il/DataTypes.hpp"
#include "il/ILOpCodes.hpp"
#include "ilgen/IlGeneratorMethodDetails_inlines.hpp"
#include "ilgen/IlInjector.hpp"
#include "ilgen/TypeDictionary.hpp"

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
}
