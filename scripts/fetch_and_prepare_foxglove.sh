#!/usr/bin/env bash
set -euo pipefail

# Usage: fetch_and_prepare_foxglove.sh <url> <sha256> [<dest_dir>]
URL="${1:-}"
SHA256_EXPECTED="${2:-}"
DEST_DIR="${3:-}"

# 如果未提供 dest_dir，默认为项目根目录下 thirdparty/foxglove-src（脚本位于 scripts/）
if [[ -z "$DEST_DIR" ]]; then
  SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
  PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
  DEST_DIR="${PROJECT_ROOT}/thirdparty/foxglove-src"
fi

if [[ -z "$URL" || -z "$SHA256_EXPECTED" ]]; then
  echo "Usage: $0 <url> <sha256> [<dest_dir>]" >&2
  exit 2
fi

TMPDIR="$(mktemp -d)"
TMPZIP="$TMPDIR/foxglove_sdk.zip"
TMPEXTRACT="$TMPDIR/extract"

cleanup() {
  rm -rf "$TMPDIR"
}
trap cleanup EXIT

echo "Downloading Foxglove SDK from: $URL"
if command -v curl >/dev/null 2>&1; then
  curl -L --fail -o "$TMPZIP" "$URL"
elif command -v wget >/dev/null 2>&1; then
  wget -O "$TMPZIP" "$URL"
else
  echo "Error: neither curl nor wget is available" >&2
  exit 3
fi

echo "Verifying SHA256..."
if command -v sha256sum >/dev/null 2>&1; then
  SHA256_ACTUAL="$(sha256sum "$TMPZIP" | awk '{print $1}')"
elif command -v shasum >/dev/null 2>&1; then
  SHA256_ACTUAL="$(shasum -a 256 "$TMPZIP" | awk '{print $1}')"
else
  echo "Error: neither sha256sum nor shasum is available" >&2
  exit 4
fi

if [[ "$SHA256_ACTUAL" != "$SHA256_EXPECTED" ]]; then
  echo "SHA256 mismatch: expected $SHA256_EXPECTED but got $SHA256_ACTUAL" >&2
  exit 5
fi

echo "Extracting SDK..."
mkdir -p "$TMPEXTRACT"
if command -v unzip >/dev/null 2>&1; then
  unzip -q "$TMPZIP" -d "$TMPEXTRACT"
else
  echo "Error: unzip is required but not found" >&2
  exit 6
fi

# Locate the extracted SDK root containing include/ and lib/
FOUND_DIR=""
for d in "$TMPEXTRACT"/*; do
  if [[ -d "$d/include" && -d "$d/lib" ]]; then
    FOUND_DIR="$d"
    break
  fi
done

# If not found at top-level, maybe zip content already at root
if [[ -z "$FOUND_DIR" ]]; then
  if [[ -d "$TMPEXTRACT/include" && -d "$TMPEXTRACT/lib" ]]; then
    FOUND_DIR="$TMPEXTRACT"
  fi
fi

if [[ -z "$FOUND_DIR" ]]; then
  echo "Error: could not locate SDK root containing include/ and lib/ in the archive" >&2
  exit 7
fi

echo "Preparing destination: $DEST_DIR"
mkdir -p "$(dirname "$DEST_DIR")"
rm -rf "$DEST_DIR"
mv "$FOUND_DIR" "$DEST_DIR"

echo "Foxglove SDK prepared at: $DEST_DIR"
echo "include dir: $DEST_DIR/include"
echo "lib file (expected): $DEST_DIR/lib/libfoxglove.a"

exit 0
