# nrio-usb-examples

TinyUSB examples ported to the NDS using the NRIO/DS-Writer Slot-2 USB controller.

Requires [BlocksDS](https://blocksds.github.io/docs/) to build.

## Programming notes

- When adapting existing TinyUSB code, `tud_descriptor_device_cb` must be modified
  to set the endpoint 0 max packet size at runtime; this is because the expected
  maximum packet size differs between D14 and D12 boards. Add the following line:

`desc_device.bMaxPacketSize0 = tud_edpt0_get_size();`

- D14 boards support up to 7 data endpoints with arbitrary configuration. D12 boards
  only support 2 data endpoints, of which endpoint 2 provides larger buffers, support
  for acting as an isochronous endpoint, and double-buffering. As such, it is
  recommended to put the most transfer-intensive endpoint at number 2.

## License

MIT
