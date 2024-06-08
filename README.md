# THIS IS ONLY FOR WT32-SC01-PLUS 
# (NOT WT32-SC01)

## ![Screenshot](/images/screen.png)
## ![Screenshot](/images/Screen2.jpg)

## "release" contains the firmware.

### --chip esp32s3
### --port "/dev/cu.usbmodem101"  (replace with your port !)
### --baud 921600 
### --before default_reset 
### –-after hard_reset write_flash -z 
### --flash_mode dio 
### --flash_freq 80m 
### --flash_size 16MB

### 0x0000 bootloader.bin

### 0x8000 partitions.bin 

### 0xe000 boot_app0.bin 

### 0x10000 firmware.bin

