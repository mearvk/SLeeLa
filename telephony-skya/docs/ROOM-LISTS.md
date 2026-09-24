# Skya™ Room Lists

## Purpose

Skya™ supports portable room lists for sharing server endpoints and room metadata between clients. A room entry contains:

- Room name
- Host or IP address
- Port
- DNS name
- Description

The Remote Connection GUI presents the room name as a dropdown. Selecting a room automatically fills its host/IP and port.

## Built-in rooms

The client starts with 40 ordinary room names, including Lobby, General, Welcome, Community, Work, Study, Science, Technology, Programming, Linux, Networking, Security, Music, Games, Travel, Projects, Support and Help.

These are local defaults. They do not imply that a remote server exists at the listed endpoint.

## Create and distribute a list

The **Rooms** menu provides:

- **Add Room** — create an entry with name, IP/host, port, DNS name and description.
- **Remove Selected** — remove the current entry.
- **Export Room List** — write the current list for distribution.
- **Import Room List** — replace the current list with a shared list.

A room list can therefore be prepared by an administrator and distributed as an ordinary file.

## Supported formats

### XML

    <skya-room-list>
      <room>
        <name>Lobby</name>
        <host>192.0.2.10</host>
        <port>8443</port>
        <dns>skya.example.net</dns>
        <description>General discussion</description>
      </room>
    </skya-room-list>

### JSON

    {
      "rooms": [
        {
          "name": "Lobby",
          "host": "192.0.2.10",
          "port": 8443,
          "dns": "skya.example.net",
          "description": "General discussion"
        }
      ]
    }

### TXT

Plain text uses:

    # name|host|port|dns|description
    Lobby|192.0.2.10|8443|skya.example.net|General discussion

### Markdown

Markdown uses a table:

    | Room | Host/IP | Port | DNS | Description |
    |---|---|---:|---|---|
    | Lobby | 192.0.2.10 | 8443 | skya.example.net | General discussion |

## Distribution and security

Room lists are endpoint metadata, not credentials. They should not contain passwords, private keys, authentication tokens, or other secrets. IP addresses and DNS names can disclose network topology, so lists should only be distributed to intended recipients.

The list format does not itself establish that an endpoint is reachable or trustworthy. The client still performs its normal connection process, including the five-second initial connection timeout.

## Implementation

The JavaFX client uses `SkyaRoomListManager` for import/export and `SkyaConnectApp` for room selection and endpoint population. HTTP/2 and HTTP/3 remain protocol selections at the GUI boundary; selecting them does not by itself make the underlying TCP foundation an HTTP/2 or HTTP/3 implementation.
