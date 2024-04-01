# ZMK Mouse Key Toggle Behavior

This is a version of [Key Toggle](https://zmk.dev/docs/behaviors/key-toggle) for [Mouse Key Press/Release](https://zmk.dev/docs/behaviors/mouse-emulation#behavior-binding).

## What it does

As titled. Made this for toggling `Click and Drag` in [mac-mouse-fix.app](https://github.com/noah-nuebling/mac-mouse-fix).

## Installation

Include this project on your ZMK's west manifest in `config/west.yml`:

```yaml
manifest:
  remotes:
    #...
    - name: badjeff
      url-base: https://github.com/badjeff
    #...
  projects:
    #...
    - name: zmk-behavior-mouse-key-toggle
      remote: badjeff
      revision: main
    #...
```

Add the behavior in `shield.keymap`.

```keymap
#include <behaviors/mouse_key_toggle.dtsi> // for &mktg
/ {
        keymap {
                compatible = "zmk,keymap";
                default_layer {
                        bindings = <
                              ..... .... .... &mktg MCLK .... .... .....
                        >;
                };
       };

};
```

Enable input subsystem in your `<shield>.config` file:

```conf
CONFIG_INPUT=y
```
