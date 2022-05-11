#!/bin/bash

make CPPFLAGS='-Wall -Wextra -Werror -std=c++98 -D STD' NAME='std.out';
make CPPFLAGS='-Wall -Wextra -Werror -std=c++98' NAME='ft.out';

echo "Executing std.out, storing output in std.log"
./std.out > std.log
echo "Executing ft.out, storing output in ft.log"
./ft.out > ft.log
echo "Storing diff in diff.log"
diff std.log ft.log > diff.log