# SLeeLa Governance Source Models

This package defines source-level shapes for voluntary manual project-governance votes. The proposed source annotation is documented in ../../registry/DEMOCRATIC-VOTING-SYNTAX.md.

The annotation uses the existing SLeeLa documentation-comment form with an @vote sentinel. It is inert documentation, not a compiler language feature or executable voting command today.

Source models:
- SLVoteProposal — frozen source reference, electorate, voting window, counting method, quorum, privacy and teller policy.
- SLVoteBallot — anonymous choice record with no voter identity field.
- SLVoteTally — aggregate counts, quorum calculation and certification reference.

The manual process separates the eligibility roster from secret paper ballots. A public record contains aggregate counts and non-identifying audit references only. A proposal revision is frozen by SHA-256; any substantive amendment requires a new vote ID.

This is for internal or voluntary organizational governance. It is not represented as a statutory public-election service, legally binding ballot, or voter-authentication system. The current compiler does not parse @vote blocks and no automated ballot collection is provided.
