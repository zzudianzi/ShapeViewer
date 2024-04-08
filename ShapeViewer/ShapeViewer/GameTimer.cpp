#include "pch.h"

#include "GameTimer.h"

GameTimer::GameTimer()
    : _SecondsPerCount(0.),
      _DeltaTime(-1.),
      _BaseTime(0),
      _PausedTime(0),
      _PrevTime(0),
      _CurTime(0),
      _Stopped(false),
      _StopTime(0)
{
    __int64 countPerSec;
    QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&countPerSec));
    _SecondsPerCount = 1. / static_cast<double>(countPerSec);
}

double GameTimer::TotalTimeInSeconds() const
{
    double validTime = static_cast<double>(_Stopped ? _StopTime : _CurTime);
    return (validTime - _PausedTime - _BaseTime) * _SecondsPerCount;
}

double GameTimer::DeltaTimeInSeconds() const
{
    return _DeltaTime;
}

void GameTimer::Reset()
{
    __int64 curTime;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&curTime));

    _BaseTime = curTime;
    _PrevTime = curTime;
    _StopTime = 0;
    _Stopped = false;
}

void GameTimer::Start()
{
    __int64 startTime;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&startTime));

    if (_Stopped)
    {
        _PausedTime += (startTime - _StopTime);

        _PrevTime = startTime;
        _StopTime = 0;
        _Stopped = false;
    }
}

void GameTimer::Stop()
{
    if (!_Stopped)
    {
        __int64 curTime;
        QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&curTime));

        _StopTime = curTime;
        _Stopped = true;
    }
}

void GameTimer::Tick()
{
    if (_Stopped)
    {
        _DeltaTime = 0.;
        return;
    }

    // Get the time this frame
    __int64 curTime;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&curTime));
    _CurTime = curTime;

    // Time difference between this frame and the previous
    _DeltaTime = (_CurTime - _PrevTime) * _SecondsPerCount;

    // Prepare for next frame
    _PrevTime = _CurTime;

    // Force nonnegative. The DXSDK's CDXUTTimer mentions that if the processor goes into a power
    // save mode or we get shuffled to another processor, the _DeltaTime can be negative.
    if (_DeltaTime < 0.)
    {
        _DeltaTime = 0.;
    }
}
