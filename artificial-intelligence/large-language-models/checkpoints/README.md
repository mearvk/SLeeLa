# Checkpoints

Checkpoint creation is requested through `sllmt_trainer_save_checkpoint`; serialization is backend-defined. Store checkpoints separately from immutable source datasets and released model packages. Use unique run identifiers, record the model and tokenizer versions, training configuration, dataset provenance, step counters, and checksums.

Treat checkpoints as untrusted binary input when loading them. Enforce file-size and tensor-dimension limits, verify integrity, and never deserialize executable content from an untrusted source.
