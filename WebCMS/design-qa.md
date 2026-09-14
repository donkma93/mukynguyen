# Design QA — Login history summary

- Source visual truth: `C:\Users\donpv\AppData\Local\Temp\codex-clipboard-794a2d76-5d08-4d9a-b366-864c6f10f17e.png`
- Intended implementation route: `/panel/login-history`
- Intended state: authenticated player with a public-IP login record and a GameServer authentication record.
- Source dimensions: 880 × 150 px.
- Implementation screenshot: unavailable — the route correctly requires an authenticated player session, and no credentials were supplied for visual testing.
- Viewport / density normalization: blocked pending authenticated capture at desktop width.

## Full-view comparison evidence

Blocked. The source was inspected; the implementation could not be captured in the required authenticated state without using a player credential.

## Focused region comparison evidence

Blocked for the same reason. The target region is the three-column summary row: IP + Check-Host link, Game last authentication, and Website last authentication.

## Findings

- [P2] Authenticated visual comparison remains pending.
  - Evidence: `/panel/login-history` redirects unauthenticated requests to `/login`.
  - Impact: the final browser-rendered layout cannot yet be judged against the reference at the matching state.
  - Fix: sign in with a player account, then capture the login-history route and compare the three-column row with the source image.

## Required fidelity surfaces

- Fonts and typography: implementation uses the product's existing font stack; authenticated rendering not captured.
- Spacing and layout rhythm: implemented as a compact three-column desktop table; authenticated rendering not captured.
- Colors and visual tokens: summary uses the source's deep navy body and muted purple header; authenticated rendering not captured.
- Image quality and assets: the reference contains no required image assets; none were substituted.
- Copy and content: localized labels distinguish IP, Game, Website, and last authentication.

## Implementation checklist

- [x] Add the direct Check-Host IP information URL beside each public IP.
- [x] Add GameServer last-authentication data beside Website last-authentication data.
- [x] Preserve the detailed 30-day website-IP history beneath the summary.
- [ ] Capture and compare the authenticated route after a player signs in.

## Comparison history

- Iteration 1: source inspected; implementation capture blocked by the authenticated route requirement.

final result: blocked
