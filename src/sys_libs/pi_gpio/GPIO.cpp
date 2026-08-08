#include "GPIO.hpp"

GpioChip::GpioChip() {
    _chip = gpiod_chip_open("/dev/gpiochip0"); // gpiochip4 on Pi 5
    if (!_chip)
        perror("gpiod_chip_open");
}

GpioChip::~GpioChip() {
    if (_chip)
        gpiod_chip_close(_chip);
}

GpioLine::GpioLine(const GpioChip& chip, unsigned offset) : offset_(offset) {
    if (!chip.ok())
        return;

    settings_ = gpiod_line_settings_new();
    if (!settings_) {
        perror("gpiod_line_settings_new");
        return;
    }
    gpiod_line_settings_set_direction(settings_, GPIOD_LINE_DIRECTION_OUTPUT);
    gpiod_line_settings_set_output_value(settings_, GPIOD_LINE_VALUE_INACTIVE);

    line_cfg_ = gpiod_line_config_new();
    if (!line_cfg_) {
        perror("gpiod_line_config_new");
        return;
    }

    if (gpiod_line_config_add_line_settings(line_cfg_, &offset_, 1, settings_) != 0) {
        perror("gpiod_line_config_add_line_settings");
        return;
    }

    req_cfg_ = gpiod_request_config_new();
    if (!req_cfg_) {
        perror("gpiod_request_config_new");
        return;
    }
    gpiod_request_config_set_consumer(req_cfg_, "gpio_line");

    request_ = gpiod_chip_request_lines(chip.handle(), req_cfg_, line_cfg_);
    if (!request_)
        perror("gpiod_chip_request_lines");
}

GpioLine::~GpioLine() {
    if (request_) {
        gpiod_line_request_set_value(request_, offset_, GPIOD_LINE_VALUE_INACTIVE);
        gpiod_line_request_release(request_);
    }
    if (req_cfg_)  gpiod_request_config_free(req_cfg_);
    if (line_cfg_) gpiod_line_config_free(line_cfg_);
    if (settings_) gpiod_line_settings_free(settings_);
}

void GpioLine::set(gpiod_line_value value) {
    gpiod_line_request_set_value(request_, offset_, value);
}

void GpioLine::setB(bool bValue) {
    GpioLine::set((bValue) ?
        GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE);
}
