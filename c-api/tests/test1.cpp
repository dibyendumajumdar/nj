#include "nj_api.h"

#include <stdio.h>

static bool build_il(JIT_ILInjectorRef ilinjector, void *userdata) {
  JIT_CreateBlocks(ilinjector, 1);
  JIT_SetCurrentBlock(ilinjector, 0);
  auto iconst = JIT_ConstInt32(42);
  auto node = JIT_CreateNode1C(OP_ireturn, iconst);
  JIT_GenerateTreeTop(ilinjector, node);
  JIT_CFGAddEdge(ilinjector, JIT_BlockAsCFGNode(JIT_GetCurrentBlock(ilinjector)), JIT_GetCFGEnd(ilinjector));
  return true;
}

int main(int argc, const char *argv[]) {

  JIT_ContextRef ctx = JIT_CreateContext();
  if (ctx) {
    JIT_FunctionBuilderRef function_builder = JIT_CreateFunctionBuilder(
        ctx, "ret1", JIT_Int32, 0, NULL, build_il, NULL);

    typedef int (*F)(void);
    F f = (F)JIT_Compile(function_builder);
    if (f)
      printf("Function call returned %d\n", f());

    JIT_DestroyFunctionBuilder(function_builder);
  }
  JIT_DestroyContext(ctx);
  return 0;
}
