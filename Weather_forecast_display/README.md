# Weather forecast display info and requirements
## The LocationForecast API
The API used to get weather data is from Norway ****Meteorologisk institutt****, they have a variety of different endpoints, this project uses locationForecast 2.0.

### Usage and Terms of service
They have a certain policy and [TOS](https://api.met.no/doc/TermsOfService) that must be followed when using this API. You must read, understand and correctly set the values in [the template](./values_template.h).

You can read their terms of service [here](https://api.met.no/doc/TermsOfService)

## Values file and template
In order to not have values directly in the code, the program includes a file named values.h, there is a [template](./values_template.h) of this file, in order for the program to function properly this file must be renamed (or create a new file and copy values) to `values.h`. The values must also be filled out with information such as your location with latitude and longitude coordinates, network information and a fitting user agent that follows the requirements of the [TOS](https://api.met.no/doc/TermsOfService).

## Font and symbols
The API also provides a [symbol code summary](https://github.com/metno/weathericons) with accompanying symbols. Because a simple OLED display for microcontrollers can not load and display images in any easy, there is a [font](./WeatherIcon.h) that is set beforing showing the symbols. The symbol code is just matched and mapped to the correct character that looks like the corresponding symbol. (For example **N** is not an **N** but rather a symbol for a cloud, so cloudy would map to N and this is printed on the display)

I originally sourced the set of icons (the font) from [kaldoran](https://github.com/kaldoran/Adafruit-Weather-Icon) on github, but slightly modified them.

If you would like to modify or just see what the different characters look like, copy the contents from the [font file](./WeatherIcon.h) into [this](https://tchapi.github.io/Adafruit-GFX-Font-Customiser/) website.

To be able to use the font, make sure that the file `WeatherIcon.h`is inside your `Adafruit_GFX_Library/Fonts` folder. Typically found under your sketchbook location. `.../Arduino/libraries/Adafruit_GFX_Library/Fonts`