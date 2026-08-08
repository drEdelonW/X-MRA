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
/*============================================================*/

GpioLine::GpioLine(
    const GpioChip& chip,
    unsigned offset,
    gpiod_line_direction pinDir
):
    _pinNum(offset),
    _pinDir(pinDir)
{
    if (!chip.ok())
        return;

    if (_lineSettings = gpiod_line_settings_new()) {
        gpiod_line_settings_set_direction(_lineSettings, _pinDir);
        if (_pinDir == GPIOD_LINE_DIRECTION_OUTPUT)     gpiod_line_settings_set_output_value(_lineSettings, GPIOD_LINE_VALUE_INACTIVE);

        if (_lineCfg = gpiod_line_config_new())
        if (gpiod_line_config_add_line_settings(_lineCfg, &_pinNum, 1, _lineSettings) == 0)
        if (_rqCfg = gpiod_request_config_new()) {
            gpiod_request_config_set_consumer(_rqCfg, "gpio_line");
            if (_rq = gpiod_chip_request_lines(chip.handle(), _rqCfg, _lineCfg)) {
                // ok
            }
            else perror("gpiod_chip_request_lines()");
        }   else perror("gpiod_request_config_new()");
            else perror("gpiod_line_config_add_line_settings()");
            else perror("gpiod_line_config_new()");
    }       else perror("gpiod_line_settings_new()");
}

GpioLine::~GpioLine() {
    if (_rq) {
        gpiod_line_request_set_value(_rq, _pinNum, GPIOD_LINE_VALUE_INACTIVE);
        gpiod_line_request_release(_rq);
    }
    if (_rqCfg)         gpiod_request_config_free(_rqCfg);
    if (_lineCfg)       gpiod_line_config_free(_lineCfg);
    if (_lineSettings)  gpiod_line_settings_free(_lineSettings);
}

gpiod_line_value GpioLine::set(gpiod_line_value value) {
    return (
        (_pinDir == GPIOD_LINE_DIRECTION_OUTPUT) &&
        ((gpiod_line_request_set_value(_rq, _pinNum, value)) != 0)
    )?
        value : GPIOD_LINE_VALUE_ERROR;
}

bool GpioLine::setB(bool bValue) {
    if (_pinDir == GPIOD_LINE_DIRECTION_OUTPUT) {
       return (
            GpioLine::set(
                (bValue) ?
                    GPIOD_LINE_VALUE_ACTIVE :
                    GPIOD_LINE_VALUE_INACTIVE
            ) == GPIOD_LINE_VALUE_ACTIVE);
    }
    perror("GPIOD_LINE_DIRECTION_INPUT");
    return false;
}
