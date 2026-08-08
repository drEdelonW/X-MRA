#pragma once

#include <gpiod.h>
#include <cstdio>
#include <unistd.h>

class GpioChip {
  public:
    explicit GpioChip();
    ~GpioChip();

    GpioChip(const GpioChip&) = delete;
    GpioChip& operator=(const GpioChip&) = delete;

    gpiod_chip* handle() const { return _chip; };
    bool ok() const { return _chip != nullptr; };

  private:
    gpiod_chip* _chip = nullptr;
};
/*============================================================*/

class GpioLine {
  public:
    GpioLine(const GpioChip& chip, unsigned offset, gpiod_line_direction pinDir = GPIOD_LINE_DIRECTION_OUTPUT);
    ~GpioLine();

    GpioLine(const GpioLine&) = delete;
    GpioLine& operator=(const GpioLine&) = delete;

    bool ok() const { return _rq != nullptr; }

    gpiod_line_value  set(gpiod_line_value value);
    bool              setB(bool bValue);

  private:
    unsigned              _pinNum;
    gpiod_line_direction  _pinDir;
    gpiod_line_settings*  _lineSettings = nullptr;
    gpiod_line_config*    _lineCfg = nullptr;
    gpiod_request_config* _rqCfg = nullptr;
    gpiod_line_request*   _rq = nullptr;
};
