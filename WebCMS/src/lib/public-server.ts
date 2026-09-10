/** Public ConnectServer endpoint shown on the site. Override via .env.local. */
export function getPublicServerHost(): string {
  return (process.env.NEXT_PUBLIC_SERVER_IP || "127.0.0.2").trim() || "127.0.0.2";
}

export function getPublicServerPort(): string {
  return (process.env.NEXT_PUBLIC_SERVER_PORT || "44405").trim() || "44405";
}

export function getPublicServerEndpoint(): string {
  return `${getPublicServerHost()}:${getPublicServerPort()}`;
}
