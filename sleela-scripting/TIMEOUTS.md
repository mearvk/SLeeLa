# Sleela Script Timeouts

All Turing 5 contexts are finite-duration contexts.

## Syntax

    timeout 30 seconds
    timeout 15 minutes
    timeout 2 hours
    timeout 3 days

The timeout is a deadline, not a request for indefinite execution.

## Host limits

Hosts may define default timeout, maximum timeout, grace period, CPU budget, memory budget, evaluation budget, output budget and queue budget.

A script may lower its deadline but cannot raise the host maximum.

## Cancellation

Cancellation is cooperative where possible and forceful at the host boundary when required. Native calls must expose cancellation points or bounded-operation contracts.

## Long scientific work

Use checkpointed batches:

    checkpoint.load()
    calculate_batch()
    checkpoint.save()
    queue.push(...)

This permits a multi-day calculation to resume without becoming a permanent modification to the SLeeLa document.
