# SLeeLa Source Voting Annotation — Manual Democratic Governance

**Status:** proposed declarative syntax for Syntax 3.0 source documents. The @vote block below is ordinary SLeeLa documentation-comment content. It is inert: the current compiler does not parse it, collect ballots, determine eligibility, or produce a legally binding election result. A future governance tool may validate and manage it without changing program execution.

## 1. Design goals

A vote defined in a SLeeLa source document must be tied to an exact source revision and a clearly stated proposal; limited to an expressly named, independently defined electorate; open and closed at explicit UTC times; governed by a declared counting rule, quorum, and treatment of abstentions; suitable for paper/manual ballots and a separately maintained eligibility register; auditable without publishing secret ballots or exposing voter identities; certified by independent tellers; and distinguishable from ordinary code comments, runtime instructions, and legal elections.

The annotation records how a human decision is to be conducted. It is not itself a vote and never authorizes code execution, deployment, spending, or policy changes.

## 2. Proposed source syntax

Use a documentation comment with the sentinel @vote. Since the enclosing << ... >> block is already comment syntax, older compilers ignore the entire declaration safely.

    #sleela 3.0
    <<
    @vote
    id: "registry-record-model-2026-01"
    title: "Adopt the Registry 3.0 record model"
    proposal: "Approve the versioned identity and relationship record model."
    source: "registry/REGISTRY-3.0-ARCHITECTURE.md"
    source_digest: "sha256:0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    electorate: "RegistryMaintainers"
    eligibility_rule: "registry-maintainer-v1"
    opens_utc: "2026-11-05T15:00:00Z"
    closes_utc: "2026-11-12T21:00:00Z"
    ballot: "manual-paper"
    secrecy: "secret"
    options: "approve,reject,abstain"
    counting_rule: "strict-majority"
    quorum_min_ballots: 5
    quorum_min_participation_percent: 50
    abstain_counts_for_quorum: true
    abstain_in_approval_denominator: false
    tie_result: "not-adopted"
    invalid_ballot: "exclude-and-record-count"
    tellers_required: 2
    challenge_hours: 72
    result_publication: "aggregate-only"
    >>
    material;
    class RegistryRecordModelProposal {
      String proposalId = "registry-record-model-2026-01";
      String sourceDigest = "sha256:0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef";
    }

The digest above is an illustrative placeholder, not a real digest. Before opening a ballot, replace it with the SHA-256 digest of the exact frozen proposal source. The source path is a locator; the digest plus source revision identifies the text that voters considered.

## 3. Annotation fields

| Field | Required meaning |
|---|---|
| id | Unique proposal/vote identifier in the governance namespace. Never reused. |
| title, proposal | Human-readable title and exact question/decision proposed. |
| source | Repository-relative path or stable URI to the complete proposal text. |
| source_digest | SHA-256 of the frozen proposal bytes, with declared encoding/newline convention. |
| electorate | Named group or roster reference; never inferred from repository access. |
| eligibility_rule | Versioned policy describing admission to the electorate and the record date. |
| opens_utc, closes_utc | Explicit UTC window; close instant is exclusive. |
| ballot | manual-paper for hand-marked and manually counted ballots. |
| secrecy | secret or open. No default. |
| options | Exact allowed choices. The base profile is approve/reject/abstain. |
| counting_rule | strict-majority or declared supermajority such as two-thirds. |
| quorum_min_ballots | Minimum valid ballots required, including abstentions when configured. |
| quorum_min_participation_percent | Optional minimum valid-ballot participation percentage of the frozen eligible electorate. |
| abstain_counts_for_quorum | Whether abstentions count toward participation quorum. |
| abstain_in_approval_denominator | Whether abstentions enter the approve/reject denominator. Recommended false. |
| tie_result | Required tie outcome; baseline is not-adopted. |
| invalid_ballot | Handling of invalid, duplicate, late, or spoiled submissions. |
| tellers_required | Minimum independent tellers; secret manual ballots require at least two. |
| challenge_hours | Time to challenge procedure or arithmetic after preliminary publication. |
| result_publication | aggregate-only for secret voting; no voter-to-choice mapping is public. |

A parser must reject missing mandatory fields, duplicate fields, unknown counting rules, invalid percentages, malformed timestamps, a missing source digest, or a close time not later than the open time. It must not invent defaults for electorate, ballot secrecy, quorum, or threshold.

## 4. Manual secret-ballot procedure

1. Freeze the proposal. Publish the exact source revision, digest, proposal question, voter eligibility policy, and voting window. Any substantive amendment creates a new proposal ID and restarts the vote.
2. Freeze the electorate. A designated eligibility custodian produces a dated roster before opening. Record the roster digest and eligible count. Repository read access, commit access, or authorship alone does not establish eligibility.
3. Issue one ballot per eligible participant. The custodian marks participation on a separate roster. For secret voting, do not put a person's name, account, signature, or roster identifier on the ballot. Mix submitted paper ballots before counting. Do not create a public mapping between voter and ballot.
4. Secure custody. Two custodians witness ballot-box sealing, opening, transfer, and count. Record issued, returned, spoiled, late, and invalid ballot totals separately.
5. Count independently. Two tellers count the same set independently. Reconcile discrepancies before certifying; retain a signed count sheet and non-identifying ballot-handling log.
6. Publish the result. Publish electorate size, valid participation, approve, reject, abstain, invalid, quorum result, threshold, arithmetic, source digest, teller attestations, and challenge deadline. For secret votes, publish aggregates only.
7. Challenge and certify. Challenges identify a procedural or arithmetic issue and are resolved under the stated governance policy. Record the final certificate, dissent, and unresolved qualifications. A challenge does not reveal secret ballots.
8. Record the outcome. Registry may catalog the proposal, aggregate tally, source digest, and certification reference. A successful vote is a governance decision only within the authority of the named organization and its rules; it is not general legal authorization.

## 5. Counting rules

Let E be the eligible participants in the frozen roster; A the valid approve ballots; R the valid reject ballots; B the valid abstain ballots; V the valid ballots counted for quorum (A + R + B when abstentions count for quorum); Q the required quorum; and D the approval denominator (A + R when abstentions are excluded).

Quorum is met only when V is at least Q. If quorum fails, the result is no decision. If D is zero, the result is no decision even when the participation quorum is met.

- Strict majority: adopt only when A is greater than R. A tie is not adopted.
- Two-thirds: adopt only when A is at least the ceiling of two times D divided by three.
- Abstentions are reported and count toward participation only when configured. They are not silently counted as approval or rejection.

No outcome is inferred from a raw percentage without reporting quorum, actual counts, denominator, and method.

## 6. Open ballots

For secrecy: open, the organization may publish voter identities and choices only when its eligibility policy and participants' notice explicitly permit disclosure. The default privacy posture should be secret. Never switch a vote from secret to open after it begins.

## 7. Registry integration model

A future governance adapter may create:
- governance.proposal — frozen proposal identity, source digest, electorate reference, rule, and dates;
- governance.ballot-session — opening/closing event, custody references, and participation totals;
- governance.tally — aggregate counts, quorum calculation, method, teller attestations, challenge result, and certification state.

For a secret ballot, individual choices belong in controlled custody records; public Registry stores only aggregate results and non-identifying audit references. The eligibility roster is not published as source code. No Registry register, verify, or resolve command should reveal secret choices or convert a tally into an executable command.

## 8. Implementation boundary

This document specifies a human-readable source annotation and manual process. It is not yet a compiler grammar extension, CLI ballot collector, cryptographic voting protocol, voter-authentication system, or election service. Those require a separate implementation, threat model, accessibility review, privacy policy, and test suite. The syntax is intentionally declarative and non-executable until those pieces are separately approved and built.

The process is for voluntary project or organizational governance. It does not claim to satisfy statutes or election-administration requirements for public elections.
