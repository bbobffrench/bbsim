#!/bin/bash
# Usage: ./run_sims.sh [# of nodes] [output PNG]

node_count=$1
output_file=$2

# Clear the output file
> $output_file

for blacklist_len in 0 1 5 20
	do
	echo "Running simulations with forwarding blacklist length $blacklist_len..."
	for ttl in {0..20}
	do
		echo "	Running simulation $ttl/20..."
		packet_loss=$(bin/bbsim -t 10 -n $node_count -ttl $ttl -bl $blacklist_len -p 5)
		echo "$blacklist_len $ttl $packet_loss" >> $output_file
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
	plot_data+=$'\n'$(grep "^$blacklist_len " $output_file)
	plot_data+=$'\n'"e"
done

# Run gnuplot
gnuplot << EOF
set term pngcairo
set output "$output_file"
set title "Packet loss vs. TTL for $node_count nodes"
set xlabel "TTL"
set ylabel "Packet loss (%)"
set grid
set key left top
$plot_call
$plot_data
EOF
echo "Done."
