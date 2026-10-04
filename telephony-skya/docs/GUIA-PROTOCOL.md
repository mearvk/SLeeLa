# Guia™ Protocol — GUI ↔ SLeeLa ↔ network

Guia™ is the control protocol between the Skya JavaFX GUI and the SLeeLa Skya
client. It is a small, line-oriented, request/response protocol over TCP.

## Flow

```
JavaFX GUI  --(GUIA/1 COMMAND)-->  SkyaClient.sleela  --(SKYA/1)-->  SkyaServer.sleela
     ^                                     |                                |
     +----------(GUIA/1 EVENT)-------------+<-----------(SKYA/1 reply)------+
```

- The GUI never speaks to the network directly. Each control sends one Guia
  command to the SLeeLa client over a local control socket.
- The SLeeLa client (`sleela/SkyaClient.sleela`) is the Guia agent. It listens
  on `:8700`, dispatches each command, performs the real network communication
  over `SKYA/1` for the network commands, and returns a Guia event line.
- The upstream endpoint is `sleela/SkyaServer.sleela` (`SKYA/1` on `:8443`).

## Transport framing

- One command per TCP connection. The GUI connects, writes the command, and
  half-closes its send side so the agent's single `sockread()` sees a complete
  frame.
- The command on the wire is the bare **verb**: `GUIA/1 <COMMAND>` with no
  trailing newline. Arguments shown in the GUI (host, port, room, path, …) are
  for display/logging; the agent dispatches on the exact verb.
- The agent replies with one line `GUIA/1 <EVENT> [k=v …]` and closes the
  connection, which the GUI reads to end-of-stream.

## Command → event vocabulary

| GUI command (`GUIA/1 …`) | SLeeLa action | Event returned |
|---|---|---|
| `GUI.CREATE` | announce GUI | `CLIENT.CREATED` |
| `CLIENT.CONNECT` | **SKYA/1 call** to the server | `CLIENT.CONNECTED upstream=<SKYA/1 reply>` (or `CLIENT.ERROR reason=upstream-unreachable`) |
| `SESSION.CLOSE` | end the control session | `SESSION.CLOSED` |
| `CHAT.SEND` | **SKYA/1 relay** of a chat frame | `LISTENER.RECEIVE upstream=<SKYA/1 reply>` |
| `AUDIO.START` / `AUDIO.STOP` | audio session state | `MEDIA.AUDIO state=started|stopped` |
| `VIDEO.START` / `VIDEO.STOP` | video session state | `MEDIA.VIDEO state=started|stopped` |
| `FILE.SEND` | accept a file transfer | `FILE.ACCEPTED` |
| `MONITOR.STATUS` | report engine status | `MONITOR.STATUS state=running` |
| `PING` | liveness probe | `ACK` |
| *(anything else)* | — | `COMMAND.UNKNOWN` |

If the SLeeLa client is unreachable, the GUI synthesizes
`GUIA/1 CLIENT.OFFLINE reason=…` locally so no control blocks or throws.

## GUI control coverage

Every actionable control in `SkyaClientApp` maps to a Guia command:

- **Connect / Disconnect** → `CLIENT.CONNECT` / `SESSION.CLOSE`
- **Chat Send** → `CHAT.SEND` (a `LISTENER.RECEIVE` event is shown as a peer line)
- **Audio** Start / Mute / Stop → `AUDIO.START` / `AUDIO.START state=mute` / `AUDIO.STOP`
- **Video** Start / Camera / Stop → `VIDEO.START` / `VIDEO.START device=camera` / `VIDEO.STOP`
- **File** Send → `FILE.SEND`
- **Private group** Video / Audio → `VIDEO.START` / `AUDIO.START` targeting the group

Local-only controls (config load/save/delete, folder open, directory search,
file chooser, group-window membership, About) perform local work and do not
emit a Guia command, by design.

## Running the loop

The client launch script (`build/<os>/client.sh`) brings up the SLeeLa side
(`sleela-up.sh` → `SkyaServer.sleela` + `SkyaClient.sleela`) before launching the
GUI, and tears it down on exit (`sleela-down.sh`). To run the SLeeLa side by
hand:

```sh
export SLEELA_SHEET=SHEET.sheet SLEELA_SHA256_MANIFEST=security/sha256-manifest.json
sleela run telephony-skya/sleela/SkyaServer.sleela &
sleela run telephony-skya/sleela/SkyaClient.sleela &
```
