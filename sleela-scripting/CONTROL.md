# Sleela Script Runtime Control

Turing 5 includes explicit temporary-work controls.

## Pause and resume

    process.pause()
    process.resume()
    process.yield()

These operations apply only to a script-owned or explicitly authorized execution target. A script cannot suspend arbitrary OS processes merely by calling a function.

## VM conditions

Scripts can publish typed runtime conditions:

    vm.condition("master.sequence.index", 1842)
    vm.condition("master.sequence.id", "SEQ-1842")
    vm.condition("compiler.ready", true)

The VM remains responsible for deciding what those conditions mean operationally.

## Queues

Scripts can create typed temporary queue records:

    queue.push("calculation.results", {
        sequence: 1842,
        result: area,
        status: "derived"
    })

Queues have explicit names, schemas, capacity and retention policy. A script cannot inject arbitrary records into an undeclared queue.

## Results

    return {
        status: "complete",
        sequence: sequence,
        result: answer
    }

The host may route a result to a configured queue, caller, log or VM condition channel.
