#pragma once

class GameTimer
{
  public:
    GameTimer();

    double TotalTimeInSeconds() const;
    double DeltaTimeInSeconds() const;

    void Reset();

    void Start();
    void Stop();
    void Tick();

  private:
    double _SecondsPerCount;
    double _DeltaTime;

    __int64 _BaseTime;
    __int64 _PausedTime;
    __int64 _StopTime;
    __int64 _PrevTime;
    __int64 _CurTime;

    bool _Stopped;
};
