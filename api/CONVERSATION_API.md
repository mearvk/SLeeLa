# SLeeLa Conversation API

## Set 3 — Conversational responsibilities
Participant, Identity, Role, Message, MessageId, MessagePart, Content, Attachment, Conversation, ConversationId, Turn, Transcript, Context, ContextItem, ContextWindow, Topic, Reference, Command, CommandArgument, CommandResult, Intent, Response, ResponsePart, Reply, ResponseStatus.

Each responsibility has a C++ contract under impl/conversation and a matching native .cpp implementation boundary. The layer is provider-neutral and can serve terminal, HTTP, server, GUI, email, BODI/XML, or future model integrations.

Together with the previous 50 Common, Included classes, SLeeLa now defines 75 responsibility classes and 150 native source files across these foundational layers.

**Max Rupplin — MEARVK LLC — 2026**
