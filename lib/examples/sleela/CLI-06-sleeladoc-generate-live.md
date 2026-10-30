# SleelaDoc generation and live parsing

```sh
sleela docs --generate
sleela docs --generate build/sleela-docs
sleela docs --live crypto
```

Generation writes a plain-text corpus: class pages, package overviews, and
`TOPICS.index`. The default corpus directory is documented as
`api/sleela/docs`. `--live` forces live source parsing instead of using a
present generated corpus.
