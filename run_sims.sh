#!/bin/bash
# Usage: ./run_sims.sh [output CSV] [output PNG] [# of nodes]

output_csv=$1
output_png=$2
node_count=$3

# Clear the output file
> $output_csv

for blacklist_len in 0 1 5 20
	do
	echo "Running simulations with forwarding blacklist length $blacklist_len..."
	for ttl in $(seq 0 $((node_count / 2)))
	do
		echo "	Running simulation $((ttl + 1))/$((node_count / 2 + 1))..."
		packet_loss=$(bin/bbsim -t 10 -n $node_count -ttl $ttl -bl $blacklist_len -p 5)
		echo "$blacklist_len $ttl $packet_loss" >> $output_csv
	done
done

echo "Generating plot..."
plot_call="plot "
plot_data=""
for blacklist_len in 0 1 5 20
do
	# Plot a line for for each blacklist length
	plot_call+="'-' using 2:3 with lines title 'blacklist length = $blacklist_len'"

	# Separate each entry with a comma and space
	if [ $blacklist_len -ne 20 ]; then
		plot_call+=", "
	fi

	# Add data to be plotted
	plot_data+=$'\n'$(grep "^$blacklist_len " $output_csv)
	plot_data+=$'\n'"e"
done

# Run gnuplot
gnuplot << EOF
set term pngcairo
set output "$output_png"
set title "Packet loss vs. TTL for $node_count nodes"
set xlabel "TTL"
set ylabel "Packet loss (%)"
set grid
set key left top
$plot_call
$plot_data
EOF
echo "Done."
