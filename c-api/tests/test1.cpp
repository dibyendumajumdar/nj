#include "nj_api.h"

#include <stdio.h>

static bool test1_il(JIT_ILInjectorRef ilinjector, void *userdata) {
  JIT_CreateBlocks(ilinjector, 1);
  JIT_SetCurrentBlock(ilinjector, 0);
  auto iconst = JIT_ConstInt32(42);
  auto node = JIT_CreateNode1C(OP_ireturn, iconst);
  JIT_GenerateTreeTop(ilinjector, node);
  JIT_CFGAddEdge(ilinjector, JIT_BlockAsCFGNode(JIT_GetCurrentBlock(ilinjector)), JIT_GetCFGEnd(ilinjector));
  return true;
}

static void test1(JIT_ContextRef ctx) {
	JIT_FunctionBuilderRef function_builder = JIT_CreateFunctionBuilder(
		ctx, "ret1", JIT_Int32, 0, NULL, test1_il, NULL);

	typedef int(*F)(void);
	F f = (F)JIT_Compile(function_builder);
	if (f)
		printf("Function call returned %d\n", f());

	JIT_DestroyFunctionBuilder(function_builder);
}

static bool test2_il(JIT_ILInjectorRef ilinjector, void *userdata) {
	JIT_CreateBlocks(ilinjector, 1);
	JIT_SetCurrentBlock(ilinjector, 0);
	auto index = JIT_ConstInt32(4); // Byte offset of int32_t[1]
	auto base = JIT_LoadParameter(ilinjector, 0); // Load arg
	auto value = JIT_ArrayLoad(ilinjector, base, index, JIT_Int32); // Get arg[1]
	JIT_GenerateTreeTop(ilinjector, value); // Without TreeTop here we get the stored value below
	JIT_ArrayStore(ilinjector, base, index, JIT_ConstInt32(42)); // Set arg[1] = 42
	auto node = JIT_CreateNode1C(OP_ireturn, value);
	JIT_GenerateTreeTop(ilinjector, node);
	JIT_CFGAddEdge(ilinjector, JIT_BlockAsCFGNode(JIT_GetCurrentBlock(ilinjector)), JIT_GetCFGEnd(ilinjector));
	return true;
}

static void test2(JIT_ContextRef ctx) {
	JIT_FunctionParameter params[1] = {
		{JIT_Address, "arg1"},
	};
	JIT_FunctionBuilderRef function_builder = JIT_CreateFunctionBuilder(
		ctx, "ret2", JIT_Int32, 1, params, test2_il, NULL);

	typedef int32_t(*F)(int32_t *);
	F f = (F)JIT_Compile(function_builder);
	if (f) {
		int32_t testdata[] = { 422, -84 };
		int32_t result = f(testdata);
		printf("Function returned %d, expected -84, updated value %d\n", result, testdata[1]);
	}
	JIT_DestroyFunctionBuilder(function_builder);
}


int main(int argc, const char *argv[]) {

  JIT_ContextRef ctx = JIT_CreateContext();
  if (ctx) {
	  //test1(ctx);
	  test2(ctx);
  }
  JIT_DestroyContext(ctx);
  return 0;
}
