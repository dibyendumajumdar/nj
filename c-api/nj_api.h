#ifndef JIT_api_h
#define JIT_api_h

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The Jit Context defines a container for the Jit machinery and
 * also acts as the repository of the compiled functions. The Jit
 * Context must be kept alive as long as any functions within it are
 * needed. Deleting the Jit Context will also delete all compiled
 * functions managed by the context.
 */
typedef struct JIT_Context *JIT_ContextRef;

/**
 * Creates a Jit Context.
 */
extern JIT_ContextRef JIT_CreateContext();

/**
 * Destroys the Jit Context. Note that all compiled functions
 * managed by this context will die at this point.
 */
extern void JIT_DestroyContext(JIT_ContextRef);

/*
 * These types are used to define function argument
 * types. See JIT_create_function_builder() below for further
 * notes.
 */
enum JIT_Type {
  JIT_NoType = 0,
  JIT_Int8,
  JIT_Int16,
  JIT_Int32,
  JIT_Int64,
  JIT_Float,
  JIT_Double,
  JIT_Address,
  JIT_VectorInt8,
  JIT_VectorInt16,
  JIT_VectorInt32,
  JIT_VectorInt64,
  JIT_VectorFloat,
  JIT_VectorDouble,
  JIT_Aggregate,
};

typedef struct JIT_ILInjector * JIT_ILInjectorRef;
typedef bool (*JIT_ILBuilder)(JIT_ILInjectorRef, void *userdata);

/**
* A Function Builder is used to generate code for a single
* C function equivalent. Once the code is generated by invoking
* finalize on the builder, the builder itself can be destroyed as the
* compiled function lives on in the associated Jit Context.
*/
typedef struct JIT_FunctionBuilder *JIT_FunctionBuilderRef;

typedef struct {
  JIT_Type type;
  const char *name;
} JIT_FunctionParameter;

/**
 * Creates a new function builder object. The builder is used to construct the
 * code that will go into one function. Once the function has been defined,
 * machine code is generated by calling finalize(). After the function is
 * compiled the builder object can be thrown away - the compiled function
 * will live as long as the owning Jit Context lives.
 */
extern JIT_FunctionBuilderRef
JIT_CreateFunctionBuilder(JIT_ContextRef context, const char *name,
                          enum JIT_Type return_type, int param_count,
                          JIT_FunctionParameter *parameters,
                          JIT_ILBuilder ilbuilder, void *userdata);

/**
 * Destroys the function builder object. Note that this will not delete the
 * compiled function created using this builder - as the compiled function lives
 * in the owning Jit Context.
 */
extern void JIT_DestroyFunctionBuilder(JIT_FunctionBuilderRef);

/**
 * A value type
 */
typedef struct JIT_Node *JIT_NodeRef;
typedef struct JIT_TreeTop *JIT_TreeTopRef;
typedef struct JIT_Block *JIT_BlockRef;

extern void *JIT_Compile(JIT_FunctionBuilderRef fb);

#ifdef __cplusplus
}
#endif

#endif
