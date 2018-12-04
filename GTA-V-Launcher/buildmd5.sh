#/bin/bash

echo_formatted(){
	precut=$(echo $1 | tr -s ' ')
	hash=$(echo $precut | cut -d ' ' -f 1)
	name=$(echo $precut | cut -d ' ' -f 2)
	echo "{\"${name}\", \"${hash}\"},"
}

export -f echo_formatted


for file in $(ls -l | grep "^-" | tr -s ' ' | cut -d ' ' -f 9); do
	echo_formatted "$(md5sum "$file")"
done

allowed_dir=(x64 update)
for dir in "${allowed_dir[@]}"; do
	find "$dir" -type f -exec md5sum {} \; | xargs -L1 -I {} bash -c 'echo_formatted "{}"'
done