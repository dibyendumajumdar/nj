#include "nj_api.h"

#include "Jit.hpp"
#include "ilgen/MethodBuilder.hpp"
#include "ilgen/TypeDictionary.hpp"

#include <mutex>

namespace nj {


class JIT_Context {
public:
  JIT_Context() {}

private:
  TR::TypeDictionary types;
};


static std::mutex s_jitlock;
static volatile int s_ctxcount;

} // namespace nj

using namespace nj;

static inline JIT_contextref wrap_context(JIT_Context *p) {
  return reinterpret_cast<JIT_contextref>(p);
}

static inline JIT_Context *unwrap_context(JIT_contextref p) {
  return reinterpret_cast<JIT_Context *>(p);
}

static inline JIT_valueref wrap_value(TR::IlValue *p) {
	return reinterpret_cast<JIT_valueref>(p);
}

static inline TR::IlValue *unwrap_value(JIT_valueref p) {
	return reinterpret_cast<TR::IlValue *>(p);
}

extern "C" {

JIT_contextref JIT_create_context() {
  {
    std::lock_guard<std::mutex> g(s_jitlock);
    if (s_ctxcount == 0)
      if (!initializeJit())
        return nullptr;
    s_ctxcount++;
  }

  JIT_Context *context = new JIT_Context();

  return wrap_context(context);
}

void JIT_destroy_context(JIT_contextref ctx) {
  JIT_Context *context = unwrap_context(ctx);
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

}
