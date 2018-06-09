#include "nj_api.h"

#include <stdio.h>

static bool build_il(JIT_ILInjectorRef ilinjector, void *userdata) {
  return false;
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
