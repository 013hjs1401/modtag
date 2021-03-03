radio.onReceivedValue(function (name, value) {
    if (name == "x") {
        xvalue = value
    } else if (name == "y") {
        yvalue = value
    }
})
let rightwheel = 0
let yvalue = 0
let xvalue = 0
basic.showIcon(IconNames.No)
radio.setGroup(1)
xvalue = 0
basic.forever(function () {
    rightwheel = yvalue + xvalue
    if (rightwheel > 0) {
        kitronik_motor_driver.motorOn(kitronik_motor_driver.Motors.Motor1, kitronik_motor_driver.MotorDirection.Forward, 0)
    } else {
        kitronik_motor_driver.motorOn(kitronik_motor_driver.Motors.Motor1, kitronik_motor_driver.MotorDirection.Reverse, Math.abs(rightwheel))
    }
})
