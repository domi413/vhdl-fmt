#!/usr/bin/env python3
import xml.etree.ElementTree as ET
import argparse
import sys

def parse_benchmarks(xml_file):
    try:
        tree = ET.parse(xml_file)
        root = tree.getroot()
    except Exception as e:
        print(f"Error parsing {xml_file}: {e}")
        sys.exit(1)

    benchmarks = {}
    
    for result in root.iter('BenchmarkResults'):
        name = result.get('name')
        mean_elem = result.find('mean')
        if mean_elem is not None:
            val = float(mean_elem.get('value'))
            benchmarks[name] = val
            
    return benchmarks

def main():
    parser = argparse.ArgumentParser(description="Compare two Catch2 benchmark XML files.")
    parser.add_argument("baseline", help="Path to the baseline XML file")
    parser.add_argument("new", help="Path to the new XML file")
    args = parser.parse_args()

    baseline_data = parse_benchmarks(args.baseline)
    new_data = parse_benchmarks(args.new)

    print(f"{'Benchmark Name':<50} | {'Baseline (ns)':<15} | {'New (ns)':<15} | {'Change':<10}")
    print("-" * 100)

    all_names = sorted(set(baseline_data.keys()) | set(new_data.keys()))

    for name in all_names:
        base_val = baseline_data.get(name)
        new_val = new_data.get(name)

        base_str = f"{base_val:.2f}" if base_val is not None else "N/A"
        new_str = f"{new_val:.2f}" if new_val is not None else "N/A"
        
        change_str = "N/A"
        if base_val is not None and new_val is not None:
            diff = new_val - base_val
            pct = (diff / base_val) * 100
            change_str = f"{pct:+.2f}%"
            
            # Color coding (optional, but nice for terminal)
            if pct > 5: # Slower
                change_str = f"\033[91m{change_str}\033[0m"
            elif pct < -5: # Faster
                change_str = f"\033[92m{change_str}\033[0m"

        print(f"{name:<50} | {base_str:<15} | {new_str:<15} | {change_str:<10}")

if __name__ == "__main__":
    main()
