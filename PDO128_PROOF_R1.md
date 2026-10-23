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

## R1 compile correction (2026-10-24; not yet hardware-accepted)

An operator Device Builder compile of immutable package `34067b97` failed in
`packages/canbus.yml` because the generated C++ source did not see the
package-private `pdo128_rx_uptime_ms()` declaration. The correction obtains
monotonic milliseconds directly from ESP-IDF inside the real RX lambda:
`static_cast<uint64_t>(esp_timer_get_time() / 1000ULL)`. The package requests
the system header `<esp_timer.h>` instead of relying on a relative custom
header. The old `pdo128_proof.h` is left for historical diff/traceability but
is no longer included or called. Do not clear an operator's ESPHome cache to
mask this compile issue. A consumer's top-level `esphome.includes:` can also
explicitly request `<esp_timer.h>` to avoid package-merge ambiguity.

No change to CAN dispatch, filter, interval, Power, schema or event. This new
revision requires a fresh real Device Builder compile and corresponding private
package-lock/test updates before any OTA; the previous synthetic CI PASS is
for the previous immutable SHA only.
