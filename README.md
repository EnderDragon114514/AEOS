# AEOS project

##### By Lithium4141

# Introduction

This is an operating system based on MS-DOS, or a DOS Distro

It has a graphical interface,XMS support,Rubidium Assembly support FAT32 filesystem support

# Operating system requirements
| Hardware | Required | Suggested |
|----------|----------|-----------|
|   CPU    |Intel 8086|Intel 80286|
|   RAM    | 1085KiB  |  4096KiB  |
|   GPU    |   VGA    |    VGA    |
|   GRAM   |  16KiB   |   64KiB   |
|  Monitor |VGA 640*480|VGA 640*480|
| Keyboard |   PS/2   |   PS/2    |
|  Mouse   |   PS/2   |   PS/2    |
|  Boot    |   BIOS   |    BIOS   |
|  Disk    |  1.44MiB |    4MiB   |

# Comparison between DOS and AEOS
|      |MS-DOS|VMDOS|AEOS|
|------|------|-----|----|
|USB|×|✔|✔|
|Graphics|✔₁|×|✔|
|Mouse|✔₁|✔|✔|
|Techincal Support|×|×|✔|
|Executable|MZ|MZ|RbVM|
|High Memory/Extended Memory|✔₁|✔|✔|
|Widechars and GB-2312|✔₂|×|×|
|FAT32|×|✔|✔|
|Long Filename|✔₁|×|×|
|Internet|×|×|×|
|Sound|Buzzer|×|Buzzer|
|Package Manager|×|×|✔|
|Future Update|×|×|Manual update/Rolling update|

1:Requires a full installation of DOS7.1 or higher version

2:Requires a full installation of external CCDOS installation

# How to compile
1. You need a fully DOS environment,no matter which one you are using

2. Download,clone or copy files from this project's repositories

3. Run `makesys.bat` in the root directory of the repository

4. Copy files from `driver\` and `base\` to `output\`

5. Package all files in output into a disk image(no scripts available for this step),and add a simple DOS MBR to it,no matter which DOS it is,suggested MS-DOS

If you just want to try it,use image files from `build\`! 

