# PDO128 receive-proof package, R1 (2026-10-23)

Derived exactly from
`yoziru/esphome-zehnder-comfoair@d123d47cfcf0b57e79b059266bf4be951156bfae`.
The original GPL-3.0 license, upstream authorship and all original files remain.
Modified on 2026-10-23: `packages/dashboard.yml`, `packages/canbus.yml`;
added: `packages/pdo128_proof.yml`, `packages/pdo128_proof.h`, this notice.
These changes are the reviewed PDO128 RX producer patch, not a new controller.
Pin this package commit and the upstream external component revision. Never
deploy from a floating branch.

The existing non-RTR CAN decoder runs first. An actual two-byte PDO128 response
then emits `esphome.zehnder_pdo128_rx` over the Native API when HA state
subscription, valid device UTC and finite filtered Power are available. The
payload binds raw wire bytes, filtered Power, source identity, HA entity,
per-boot session, sequence and monotonic uptime. It is provenance, not
cryptographic attestation. HA may render the payload as a native mapping.

The proof never calls `publish_state`, forces a Power update, changes filters,
adds a CAN request or changes the existing poll schedule. The consumer must
correlate raw/filtered/HA values, filter settling, session and clocks, and fail
closed on missing or invalid evidence. Exclude precisely this diagnostic event
from Recorder, preserving other exclusions, before enabling a consumer.

Use the original Atom S3 dashboard entry point with the immutable commit.
Configure `pdo128_proof_source_id` and `pdo128_proof_power_entity` explicitly
for the actual registry; the generic default is only a placeholder. Keep API
encryption, OTA, Wi-Fi and hotspot secrets local to Device Builder. No private
device configuration or credentials are distributed here.

Isolated compilation is not hardware qualification or OTA authorization.
Review actual private merged config and firmware provenance, obtain separate
operator permission, then verify real RX/event/HA correlation and endurance.
A timer, CAN TX, connection uptime or cached Power alone is not RX proof.
Repeated RX in one UTC second needs increasing sequence and monotonic uptime.
