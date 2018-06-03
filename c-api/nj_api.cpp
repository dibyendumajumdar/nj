#include "nj_api.h"

#include "Jit.hpp"
#include "ilgen/MethodBuilder.hpp"
#include "ilgen/TypeDictionary.hpp"

#include <mutex>

namespace nj {


class NJ_Context {
public:
  NJ_Context() {}

private:
  TR::TypeDictionary types;
};


static std::mutex s_jitlock;
static volatile int s_ctxcount;

} // namespace nj

using namespace nj;

static inline nj_contextref wrap_context(NJ_Context *p) {
  return reinterpret_cast<nj_contextref>(p);
}

static inline NJ_Context *unwrap_context(nj_contextref p) {
  return reinterpret_cast<NJ_Context *>(p);
}

static inline nj_valueref wrap_value(TR::IlValue *p) {
	return reinterpret_cast<nj_valueref>(p);
}

static inline TR::IlValue *unwrap_value(nj_valueref p) {
	return reinterpret_cast<TR::IlValue *>(p);
}

extern "C" {

nj_contextref nj_create_context() {
  {
    std::lock_guard<std::mutex> g(s_jitlock);
    if (s_ctxcount == 0)
      if (!initializeJit())
        return nullptr;
    s_ctxcount++;
  }

  NJ_Context *context = new NJ_Context();

  return wrap_context(context);
}

void nj_destroy_context(nj_contextref ctx) {
  NJ_Context *context = unwrap_context(ctx);
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
