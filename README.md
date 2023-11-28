___
##### If you represent Activision and want this fork to be deleted, please contact me first.
___
# Server extension for Call of Duty (2003)
It works for the multiplayer patch 1.1  
You can get info for this game at [cod.pm](https://cod.pm/)

### This adds improvements, such as:
#### Security

- q3infoboom fix
- Directory traversal protection

#### Bug fixes

- Download stuck issue fix

#### New features

- Custom GSC functions
___
### Installation instructions

You can build this project for free using Linux.

1. From the *src* folder, enter `sudo bash build.sh -d` in a terminal.
2. Put the compiled *codextended.so* file from the *build* folder to your CoD server directory.
3. Inject *codextended.so* into *cod_lnxded* like in the [step 5](https://cod.pm/guide/a7a40b/call-of-duty-1-server-on-linux-installing-and-configuring) of the cod.pm server setup guide.

The server will now get hooked with the extension.
___
#### Notes

- 11/20/2023: Fork creation, more is planned
___
