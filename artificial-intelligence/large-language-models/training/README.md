# Training pipeline

The training API in the parent directory accepts token-ID batches through `sllmt_trainer_train_batch`. A complete application pipeline is:

1. Read bounded JSONL records through the connector/data-reader API.
2. Parse and validate the application's versioned record schema.
3. Extract prompt/completion or conversation text.
4. Apply the exact tokenizer shipped with the target model.
5. Construct input/target token-ID sequences and padded or packed batches.
6. Submit batches to the selected `sllmt_backend`.
7. Record metrics and write checkpoints to a controlled destination.

The current backend contract delegates forward/backward computation, optimizer state, scheduling, batching policy, accelerator use, checkpoint serialization, and distributed coordination to the selected backend. Do not treat raw JSONL records as token IDs.
