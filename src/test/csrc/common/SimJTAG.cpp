// See LICENSE.SiFive for license details.

#include <cstdlib>
#include "remote_bitbang.h"
#include "common.h"
#ifdef CONFIG_DIFFTEST_PERFCNT
#include "perf.h"
#endif // CONFIG_DIFFTEST_PERFCNT

remote_bitbang_t* jtag;
bool enable_simjtag;
extern "C" int jtag_tick
(
 unsigned char * jtag_TCK,
 unsigned char * jtag_TMS,
 unsigned char * jtag_TDI,
 unsigned char * jtag_TRSTn,
 unsigned char jtag_TDO
)
{
#ifdef CONFIG_DIFFTEST_PERFCNT
  difftest_calls[perf_jtag_tick] ++;
  difftest_bytes[perf_jtag_tick] += 5;
#endif // CONFIG_DIFFTEST_PERFCNT
  if (!enable_simjtag) return 0;
  if (!jtag) {
    // TODO: Pass in real port number
    jtag = new remote_bitbang_t(23334);
  }

  jtag->tick(jtag_TCK, jtag_TMS, jtag_TDI, jtag_TRSTn, jtag_TDO);

  return jtag->done() ? (jtag->exit_code() << 1 | 1) : 0;

}
