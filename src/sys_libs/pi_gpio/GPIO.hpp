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

class GpioLine {
  public:
    GpioLine(const GpioChip& chip, unsigned offset);
    ~GpioLine();

    GpioLine(const GpioLine&) = delete;
    GpioLine& operator=(const GpioLine&) = delete;

    bool ok() const { return request_ != nullptr; }

    void set(gpiod_line_value value);
    void setB(bool bValue);

  private:
    unsigned offset_;
    gpiod_line_settings*  settings_ = nullptr;
    gpiod_line_config*    line_cfg_ = nullptr;
    gpiod_request_config* req_cfg_ = nullptr;
    gpiod_line_request*   request_ = nullptr;
};
