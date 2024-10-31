set -e

mapdir="keyboards/lily58/keymaps/Edward/"

qmk json2c "$mapdir/keymapjson.json" > "$mapdir/jsonmap.c"
qmk flash -kb lily58 -km Edward