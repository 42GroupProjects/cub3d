# fuzzer-scripts.md — Automated Parser Stress Testing

Writing one invalid `.cub` map by hand proves one thing. Running 1,000 procedurally-generated broken maps proves your parser is actually airtight and earns you confidence in the 42 evaluation.

## The Bash Fuzzer (Quick)

This script generates and immediately tests 10 categories of malformed maps programmatically.

```bash
#!/bin/bash
# fuzzer.sh — place in the project root and run: bash fuzzer.sh ./cub3D

BINARY="$1"
PASS=0
FAIL=0

test_case() {
    local name="$1"
    local map="$2"
    echo "$map" > /tmp/fuzz_test.cub
    output=$("$BINARY" /tmp/fuzz_test.cub 2>&1)
    if echo "$output" | grep -q "^Error"; then
        echo "  ✅  PASS: $name"
        ((PASS++))
    else
        echo "  ❌  FAIL: $name — got: $output"
        ((FAIL++))
    fi
}

echo "=== Cub3D Parser Fuzzer ==="

test_case "No args" ""      # script trick: no file created
"$BINARY" 2>&1 | grep -q "^Error" && echo "  ✅  PASS: No args" && ((PASS++)) || { echo "  ❌  FAIL: No args"; ((FAIL++)); }

test_case "Wrong extension" "$(printf 'NO ./t.xpm\nSO ./t.xpm\nWE ./t.xpm\nEA ./t.xpm\nF 255,0,0\nC 0,0,255\n\n11111\n10001\n1N001\n10001\n11111')"
mv /tmp/fuzz_test.cub /tmp/fuzz_test.txt
"$BINARY" /tmp/fuzz_test.txt 2>&1 | grep -q "^Error" && echo "  ✅  PASS: Wrong extension" && ((PASS++)) || { echo "  ❌  FAIL: Wrong extension"; ((FAIL++)); }

test_case "Missing F color" "$(printf 'NO ./t.xpm\nSO ./t.xpm\nWE ./t.xpm\nEA ./t.xpm\nC 0,0,255\n\n11111\n1N001\n11111')"

test_case "Double player" "$(printf 'NO ./t.xpm\nSO ./t.xpm\nWE ./t.xpm\nEA ./t.xpm\nF 0,0,0\nC 0,0,0\n\n11111\n1N0N1\n11111')"

test_case "Unclosed map" "$(printf 'NO ./t.xpm\nSO ./t.xpm\nWE ./t.xpm\nEA ./t.xpm\nF 0,0,0\nC 0,0,0\n\n111\n1N0\n111')"

test_case "Invalid RGB high" "$(printf 'NO ./t.xpm\nSO ./t.xpm\nWE ./t.xpm\nEA ./t.xpm\nF 256,0,0\nC 0,0,0\n\n11111\n1N001\n11111')"

test_case "Invalid RGB negative" "$(printf 'NO ./t.xpm\nSO ./t.xpm\nWE ./t.xpm\nEA ./t.xpm\nF -1,0,0\nC 0,0,0\n\n11111\n1N001\n11111')"

test_case "Invalid char in map" "$(printf 'NO ./t.xpm\nSO ./t.xpm\nWE ./t.xpm\nEA ./t.xpm\nF 0,0,0\nC 0,0,0\n\n11111\n1N0T1\n11111')"

test_case "Space leakage" "$(printf 'NO ./t.xpm\nSO ./t.xpm\nWE ./t.xpm\nEA ./t.xpm\nF 0,0,0\nC 0,0,0\n\n11111\n1N0  \n11111')"

echo ""
echo "Results: $PASS passed, $FAIL failed out of $((PASS + FAIL)) tests"
```

## The Python Fuzzer (Extensive — 1000 random maps)

For proper stress testing, the Python fuzzer randomly corrupts valid map data.

```python
#!/usr/bin/env python3
# fuzz_1000.py — Run: python3 fuzz_1000.py ./cub3D

import subprocess, random, string, os, sys

BINARY = sys.argv[1] if len(sys.argv) > 1 else "./cub3D"
VALID_BASE = """NO ./north.xpm
SO ./south.xpm
WE ./west.xpm
EA ./east.xpm
F 220,100,0
C 0,100,200

11111111
10000001
10110001
1000N001
10001001
10110001
10000001
11111111
"""

def corrupt_map(base):
    lines = base.split('\n')
    mutations = [
        lambda ls: [l.replace('N','').replace('S','').replace('E','').replace('W','') for l in ls],  # remove player
        lambda ls: [l.replace('1','0') if random.random() > 0.5 else l for l in ls],                  # break walls
        lambda ls: [l + chr(random.randint(65,90)) if 'F ' in l else l for l in ls],                  # corrupt a config
        lambda ls: [l.replace(',', ',,') if 'F ' in l else l for l in ls],                            # double comma
        lambda ls: ls[:-2],                                                                             # truncate map
    ]
    return '\n'.join(random.choice(mutations)(lines))

passes, fails = 0, 0
for i in range(1000):
    payload = corrupt_map(VALID_BASE)
    fpath = '/tmp/fuzz_{}.cub'.format(i)
    with open(fpath, 'w') as f: f.write(payload)
    try:
        result = subprocess.run([BINARY, fpath], capture_output=True, text=True, timeout=2)
        output = result.stdout + result.stderr
        if result.returncode != 0 and 'Error' in output:
            passes += 1
        else:
            print(f"  ❌ Run {i}: Unexpected success or no Error —  returncode={result.returncode}")
            fails += 1
    except subprocess.TimeoutExpired:
        print(f"  ⚠️  Run {i}: TIMEOUT — parser hung on input")
        fails += 1
    finally:
        os.remove(fpath)

print(f"\nFuzzer complete: {passes}/1000 correctly rejected, {fails} unexpected outcomes")
```

Run `python3 fuzz_1000.py ./cub3D`. If you see any unexpected outcome, immediately save the generated map and debug your parser against it manually using VS Code.
