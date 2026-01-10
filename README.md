# Fork Client — Unofficial Telegram Messenger for Android
![image](https://raw.githubusercontent.com/Forkgram/TelegramAndroid/58938f6bbe4159b90c38d9b94c9a70d57bedf3e0/TMessagesProj/src/main/res/drawable-xxhdpi/ic_launcher.png)  
Fork Client is a fork of the official Telegram for Android application.  
[<img src="https://f-droid.org/badge/get-it-on.png"
      alt="Get it on F-Droid"
      height="80">](https://f-droid.org/app/org.forkgram.messenger)

[![Attestation](https://img.shields.io/badge/attestation-Sigstore-brightgreen?logo=sigstore&logoColor=white)](https://github.com/Forkgram/TelegramAndroid/attestations)
[![Github All Releases](https://img.shields.io/github/downloads/Forkgram/TelegramAndroid/total.svg)](https://github.com/Forkgram/TelegramAndroid/releases)

## Features:

- `Delete for everyone` option enabled by default.
- Removed pencil floating icon.
- Original message date for forwarded messages.
- Smaller header in the sidebar.
- Option to disable in-app camera.
- Option to keep unmuted unread chats right after pinned dialogs.
- See the correct full number of subscribers in groups/channels.
- Option to go to the first message of a chat.
- Quick share button for every media in private chats.
- Option to start recording video messages with the rear camera.
- Unlimited unarchived pinned chats (turns their sync off).
- Option to disable big emojis.
- Forward messages without quoting the original sender.
- Added a lot of self-destruct timer's options in secret chats.
  - Added 2, 3, 4, 5, 10, 15, 20, 30, 40 minutes.
  - Added 2, 3, 5, 8, 12, 16 hours.
  - Added 2, 3, 7 and 32 days.
- Tap on cloud GIF with pre-written text will send GIF with this text as caption.
- Tap on sticker with pre-written text will send both.
- Added upload date for profile photos.
- Added ability to see the profile info from the dialogs list via context menu.
- Added ability to see unread count when you want to mark as read multiple dialogs.
- Option to directly open the archive on pulldown
- PiP mode for YouTube's in-app player
- Added an option to show colored dots to quickly see when a person was last online  
  - Yellow dot: last seen 15 minutes ago or less  
  - Orange dot: last seen 30 minutes ago or less  
  - Red dot: last seen 60 minutes ago or less 

### Privacy Features:

- Hidden `Connecting to proxy...` string.
- Accounts names hidden from the side drawer.
- Menus to edit username/bio/name moved in the debug menu (two long tap on version section).
- Option to hide avatar/title of a chat from the dialogs list.
- Option to not send stickers information in photos.
- Some features are taken from the [Telegram FOSS](https://github.com/Telegram-FOSS-Team/Telegram-FOSS).

### Privacy:

Forkgram adds no telemetry, no analytics, no crash-reporting, and no third-party network endpoints on top of upstream Telegram for Android. All traffic goes only to Telegram's official servers.



## Downloads:
You can download binaries from Releases or from my [Telegram channel Forkgram](https://t.me/forkgram).

## Verifying release builds

Every APK published in [Releases](https://github.com/Forkgram/TelegramAndroid/releases) is built on a GitHub-hosted runner and signed with a [Sigstore](https://www.sigstore.dev/) attestation, with the signing record stored in the public [Rekor](https://docs.sigstore.dev/logging/overview/) transparency log. You can use it to confirm that a given APK was produced by this repository's CI from a specific commit.

Requirements: [GitHub CLI](https://cli.github.com/) authenticated with `gh auth login`.

```sh
gh release download <tag> -R Forkgram/TelegramAndroid -p 'ForkClient.*.apk'
gh attestation verify ForkClient.<version>.apk --repo Forkgram/TelegramAndroid
```

What the attestation proves:

- the APK bytes match the digest signed by CI (no post-build tampering);
- it was built by `.github/workflows/tandroid.yml` on a GitHub-hosted runner, from the `Forkgram/TelegramAndroid` repository (matched by numeric ID, so renames cannot spoof it);
- the exact commit SHA the build was made from, linkable on GitHub.


## Building instructions for Debian 13:
[BUILDING.md](BUILDING.md)
