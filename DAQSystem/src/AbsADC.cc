#include "AbsADC.hh"

AbsADC::AbsADC()
  : fSID(0),
    fMID(0),
    fConfig(nullptr),
    fChunkDataBuffer(),
    fTotalBCount(0),
    fCurrentTime(0),
    fCurrentTrgNumber(0),
    fMutex(),
    fEventDataSize(0)
{
}

AbsADC::AbsADC(int sid)
  : fSID(sid),
    fMID(0),
    fConfig(nullptr),
    fChunkDataBuffer(),
    fTotalBCount(0),
    fCurrentTime(0),
    fCurrentTrgNumber(0),
    fMutex(),
    fEventDataSize(0)
{
}

AbsADC::AbsADC(AbsConf * config)
  : fSID(config->SID()),
    fMID(config->MID()),
    fConfig(config),
    fChunkDataBuffer(),
    fTotalBCount(0),
    fCurrentTime(0),
    fCurrentTrgNumber(0),
    fMutex(),
    fEventDataSize(0)
{
}


void AbsADC::Bclear() { fChunkDataBuffer.clear(); }

void AbsADC::Bshrink_to_fit() { fChunkDataBuffer.shrink_to_fit(); }

bool AbsADC::Bempty() { return fChunkDataBuffer.empty(); }

int AbsADC::Bsize() { return static_cast<int>(fChunkDataBuffer.size()); }

std::unique_ptr<ChunkData> AbsADC::Bpop_front(std::chrono::milliseconds timeout)
{
  auto opt = fChunkDataBuffer.pop_front(timeout);
  if (!opt) return nullptr;
  return std::move(*opt);
}

ChunkData * AbsADC::Bfront(std::chrono::milliseconds timeout) { return fChunkDataBuffer.front_ptr(timeout); }
