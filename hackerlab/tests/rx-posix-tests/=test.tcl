

proc re-test { test_name pattern string should_match a b c d e f g h i j k } {

    set does_match [ regexp pattern string aa bb cc dd ee ff gg hh ii jj kk ]

    if {[expr $should_match != $does_match]} {
	puts "ERROR $test_name : should_match == $should_match, does_match == $does_match"
    }

}
re-test "posix-2.8.3.2-0" "[]x]" "]" 1 {0 0} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-1" "[]x]" "x" 1 {0 0} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-3" "[.]" "." 1 {0 0} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-4" "[.]" "a" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.3.2-5" "[*]" "*" 1 {0 0} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-6" "[[]" "[" 1 {0 0} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-7" "[\\]" "\\" 1 {0 0} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-11" "[abc]" "a" 1 {0 0} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-12" "[abc]" "b" 1 {0 0} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-13" "[abc]" "c" 1 {0 0} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-14" "[abc]" "d" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.3.2-15" "x[abc]" "xa" 1 {0 1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-16" "x[abc]" "xb" 1 {0 1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-17" "x[abc]" "xc" 1 {0 1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-18" "x[abc]" "xd" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.3.2-19" "[^abc]" "a" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.3.2-20" "[^abc]" "b" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.3.2-21" "[^abc]" "c" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.3.2-22" "[^abc]" "d" 1 {0 0} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-23" "x[^abc]" "xa" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.3.2-24" "x[^abc]" "xb" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.3.2-25" "x[^abc]" "xc" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.3.2-26" "x[^abc]" "xd" 1 {0 1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-27" "[[:alnum:]][[:alnum:]]*" "%abc123890XYZ=" 1 {1 12} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-28" "[[:cntrl:]][[:cntrl:]]*" "%\n\t\015\f " 1 {1 4} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-29" "[[:lower:]][[:lower:]]*" "AbcdE" 1 {1 3} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-30" "[[:lower:]][[:lower:]]*" "AbcdE" 1 {0 4} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-30" "[[:lower:]][[:lower:]]*" "AbcdE" 1 {0 4} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-31" "[[:space:]][[:space:]]*" "x \t\f\nx" 1 {1 4} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-31" "[[:space:]][[:space:]]*" "x \t\f\nx" 1 {1 4} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-32" "[[:alpha:]][[:alpha:]]*" "%abC123890xyz=" 1 {1 3} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-33" "[[:digit:]][[:digit:]]*" "%abC123890xyz=" 1 {4 9} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-34" "[[:print:]][[:print:]]*" "\n %abC12\f" 1 {1 7} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-35" "[[:upper:]][[:upper:]]*" "\n aBCDEFGHIJKLMNOPQRSTUVWXYz" 1 {3 26} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-36" "[[:upper:]][[:upper:]]*" "\n aBCDEFGHIJKLMNOPQRSTUVWXYz" 1 {2 27} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-36" "[[:upper:]][[:upper:]]*" "\n aBCDEFGHIJKLMNOPQRSTUVWXYz" 1 {2 27} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-37" "[[:blank:]][[:blank:]]*" "\na \t b" 1 {2 4} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-37" "[[:blank:]][[:blank:]]*" "\na \t b" 1 {2 4} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-38" "[[:graph:]][[:graph:]]*" "\n %abC12\f" 1 {2 7} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-39" "[[:punct:]][[:punct:]]*" "a~!@#$%^&*()_+=-`[]{};':\"|\\,./?>< " 1 {1 32} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-40" "[[:xdigit:]][[:xdigit:]]*" "-0123456789ABCDEFabcdef" 1 {1 22} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-41" "[a-z][a-z]*" "ABCabcxyzABC" 1 {3 8} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-43" "[a-a][a-a]*" "zaaaaab" 1 {1 5} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-44" "[a-a][a-a]*" "ZAAAAAB" 1 {1 5} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-44" "[a-a][a-a]*" "ZAAAAAB" 1 {1 5} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-45" "[--Z][--Z]*" "!ABC-./XYZ~" 1 {1 9} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-46" "[*--Z][*--Z]*" "!+*,---ABC" 1 {1 6} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-47" "[a-][a-]*" "xa-a--a-ay" 1 {1 8} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-48" "[a[.-.]z][a[.-.]z]*" "ba-a-a-zw" 1 {1 7} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-49" "[[.a.]-[.z.]][[.a.]-z]*" "ABCabcxyzABC" 1 {3 8} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-50" "[[.a.]-[.a.]][[.a.]-[.a.]]*" "zaaaaab" 1 {1 5} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-51" "[a-[.a.]][[.a.]-a]*" "ZAAAAAB" 1 {1 5} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-51" "[a-[.a.]][[.a.]-a]*" "ZAAAAAB" 1 {1 5} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-52" "[[.-.]-[.Z.]][[.-.]-[.Z.]]*" "!ABC-./XYZ~" 1 {1 9} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-53" "[*--Z][*-[.-.]Z]*" "!+*,---ZABC" 1 {1 7} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-54" "[[:digit:]a-z#$%][[:digit:]a-z#$%]*" "__abc#lmn012$x%yz789*" 1 {2 19} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-55" "[[:digit:]a-z#$%][[:digit:]a-z#$%]*" "__abcLMN012x%#$yz789*" 1 {2 19} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-55" "[[:digit:]a-z#$%][[:digit:]a-z#$%]*" "__abcLMN012x%#$yz789*" 1 {2 19} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-56" "[^[:digit:]a-z#$%][^[:digit:]a-z#$%]*" "abc#lmn012$x%yz789--@*,abc" 1 {18 22} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-57" "[^[:digit:]a-z#$%][^[:digit:]a-z#$%]*" "abC#LMn012$x%yz789--@*,abc" 1 {18 22} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-57" "[^[:digit:]a-z#$%][^[:digit:]a-z#$%]*" "abC#LMn012$x%yz789--@*,abc" 1 {18 22} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-58" "[^-][^-]*" "---afd*(&,ml---" 1 {3 11} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-59" "[^--Z][^--Z]*" "---AFD*(&,ml---" 1 {6 11} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-60" "[^--Z][^--Z]*" "---AFD*(&,ml---" 1 {6 9} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.2-60" "[^--Z][^--Z]*" "---AFD*(&,ml---" 1 {6 9} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.3-36" "\\(abc\\)\\1" "abcabc" 1 {0 5} {0 2} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.3.3-37" "\\(abc\\)\\1" "aBcAbC" 1 {0 5} {0 2} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.1.1-0" "a" "a" 1 {0 0} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.1.2-0" "\\." "a.b" 1 {1 1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.1.2-1" "\\[" "a[b" 1 {1 1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.1.2-2" "\\\\" "a\\b" 1 {1 1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.1.2-3" "\\(" "a(b" 1 {1 1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.1.2-4" "\\*" "a*b" 1 {1 1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.1.2-5" "\\+" "a+b" 1 {1 1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.1.2-6" "\\?" "a?b" 1 {1 1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.1.2-7" "\\|" "a|b" 1 {1 1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.1.2-8" "\\^" "a^b" 1 {1 1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.1.2-9" "\\$" "a$b" 1 {1 1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.1.3-0" "." "a" 1 {0 0} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.1.3-1" "." "\n" 1 {0 0} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-0" "abc[def]ghi" "xabcdghiy" 1 {1 7} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-1" "abc((de)(fg))hi" "xabcdefghiy" 1 {1 9} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-2" "abc*def" "xabdefy" 1 {1 5} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-3" "abc*def" "xabcdefy" 1 {1 6} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-4" "abc*def" "xabcccccccdefy" 1 {1 12} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-5" "abc(def)*ghi" "xabcghiy" 1 {1 6} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-6" "abc(def)*ghi" "xabcdefghi" 1 {1 9} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-7" "abc(def)*ghi" "xabcdefdefdefghi" 1 {1 15} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-8" "abc+def" "xabdefy" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.4.3-9" "abc+def" "xabcdefy" 1 {1 6} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-10" "abc+def" "xabcccccccdefy" 1 {1 12} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-11" "abc(def)+ghi" "xabcghiy" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.4.3-12" "abc(def)+ghi" "xabcdefghi" 1 {1 9} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-13" "abc(def)+ghi" "xabcdefdefdefghi" 1 {1 15} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-14" "abc?def" "xabdefy" 1 {1 5} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-15" "abc?def" "xabcdefy" 1 {1 6} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-16" "abc?def" "xabcccccccdefy" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.4.3-17" "abc(def)?ghi" "xabcghiy" 1 {1 6} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-18" "abc(def)?ghi" "xabcdefghi" 1 {1 9} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-19" "abc(def)?ghi" "xabcdefdefdefghi" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.4.3-20" "abc{0,1}def" "xabdefy" 1 {1 5} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-21" "abc{0,1}def" "xabcdefy" 1 {1 6} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-22" "abc{0,1}def" "xabccdefy" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.4.3-23" "abc{1,3}def" "xabdefy" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.4.3-24" "abc{1,3}def" "xabcdefy" 1 {1 6} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-25" "abc{1,3}def" "xabccdefy" 1 {1 7} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-26" "abc{1,3}def" "xabcccdefy" 1 {1 8} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-27" "abc{1,3}def" "xabccccdefy" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.4.3-28" "abc{1,}def" "xabdefy" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.4.3-29" "abc{1,}def" "xabcdefy" 1 {1 6} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-30" "abc{1,}def" "xabccdefy" 1 {1 7} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-31" "abc{1,}def" "xabcccdefy" 1 {1 8} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-32" "abc{1,}def" "xabccccdefy" 1 {1 9} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-33" "abc{3}def" "xabdefy" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.4.3-34" "abc{3}def" "xabcdefy" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.4.3-35" "abc{3}def" "xabccdefy" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.4.3-36" "abc{3}def" "xabcccdefy" 1 {1 8} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-37" "abc{3}def" "xabccccdefy" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.4.3-42" "(wee|week)(night|knights)s*" "weeknights" 1 {0 9} {0 2} {3 9} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-43" "(a|aaa)*" "aaaa" 1 {0 3} {1 3} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-44" "(a|aaa){0,100}" "aaaa" 1 {0 3} {1 3} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-45" "(wee|week)(night|knights)(s*)" "weeknights" 1 {0 9} {0 2} {3 9} {10 9} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-46" "(week|wee)(knights|night)" "weeknights" 1 {0 9} {0 2} {3 9} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-47" "(aaa|a)+" "aaaa" 1 {0 3} {1 3} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-48" "(a*)*x\\1" "aaaax" 1 {0 4} {4 3} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-49" "(a*)*x\\1(a*)" "aaaaxaa" 1 {0 6} {2 3} {7 6} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-50" "(a*)*x(\\1a*)" "aaaaxaa" 1 {0 6} {2 3} {5 6} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-51" "(a*)*x(\\1x)*(.*)" "aaaaxxyy" 1 {0 7} {4 3} {5 5} {6 7} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-52" "(a{0,}){0,}x\\1" "aaaax" 1 {0 4} {4 3} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-53" "(a{0,}){0,}x\\1(a{0,})" "aaaaxaa" 1 {0 6} {2 3} {7 6} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.3-54" "(a{0,}){0,}x(\\1x){0,}(.{0,})" "aaaaxxyy" 1 {0 7} {4 3} {5 5} {6 7} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.6-0" "a{0,1}^bc" "bc" 1 {0 1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.6-1" "a{0,1}^bc" "^bc" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.4.6-2" "a{0,1}^bc" "a^bc" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.4.6-3" "a^bc" "abc" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.4.6-4" "a^bc" "a^bc" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.4.6-5" "ab$c{0,1}" "ab" 1 {0 1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-2.8.4.6-6" "ab$c{0,1}" "ab$" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.4.6-7" "ab$c{0,1}" "ab$c" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.4.6-8" "ab$c" "abc" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.4.6-9" "ab$c" "ab\nc" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.4.6-10" "ab$c" "ab$" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-2.8.4.6-11" "ab$c" "ab$c" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-case-0" "xx" "xX" 1 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-case-0" "xx" "xX" 1 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-case-1" "xX" "xx" 1 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-case-1" "xX" "xx" 1 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-case-2" "x x" "x X" 1 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-case-2" "x x" "x X" 1 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-case-3" "x X" "x x" 1 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-case-3" "x X" "x x" 1 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-case-4" "x X" "x y x" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-case-4" "x X" "x y x" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-nosub-0" "^*x" "*x" 1 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-nosub-1" "\\(*x\\)" "*x" 1 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-nosub-3" "*x" "*x" 1 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-nosub-4" "a\\{2,5\\}" "a" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-nosub-5" "a{2,5}" "a" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-nosub-6" "a\\{2,5\\}" "aa" 1 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-nosub-7" "a{2,5}" "aa" 1 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-nosub-8" "a\\{2,5\\}" "aaa" 1 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-nosub-9" "a{2,5}" "aaa" 1 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-nosub-10" "a\\{2,5\\}" "aaaa" 1 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-nosub-11" "a{2,5}" "aaaa" 1 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-nosub-12" "a\\{2,5\\}" "aaaaa" 1 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-nosub-13" "a{2,5}" "aaaaa" 1 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-nosub-14" "a\\{2,5\\}" "aaaaaa" 1 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-nosub-15" "a{2,5}" "aaaaaa" 1 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-nosub-16" "(abcd){10,11}" "abcd" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-nosub-17" "back-tracking oriented stream-of-solution functions" "in the spec, and the back-tracking oriented stream-of-solution functions" 1 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-not_ol-0" "^abc" "abcdef" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-not_ol-1" "^abc" "xyz\nabcdef" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-not_ol-2" "^abc" "xyzabcdef" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-not_ol-3" "^abc" "\nabcdef" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-not_ol-4" "abc$" "defabc" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-not_ol-5" "abc$" "defabc\nghi" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-not_ol-6" "abc$" "defabc\n" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-not_ol-7" "^abc$" "abc" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-not_ol-8" "^abc$" "abc" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-not_ol-9" "^abc$" "abc" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-not_ol-10" "^abc$" "\nabc\n" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-not_ol-11" "^abc$" "\nabc\n" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-not_ol-12" "^abc$" "\nabc\n" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-complex-not_ol-0" "^a(b)*c" "abcdef" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-complex-not_ol-1" "^a(b)*c" "xyz\nabcdef" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-complex-not_ol-2" "^a(b)*c" "xyzabcdef" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-complex-not_ol-3" "^a(b)*c" "\nabcdef" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-complex-not_ol-4" "a(b)*c$" "defabc" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-complex-not_ol-5" "a(b)*c$" "defabc\nghi" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-complex-not_ol-6" "a(b)*c$" "defabc\n" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-complex-not_ol-7" "^a(b)*c$" "abc" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-complex-not_ol-8" "^a(b)*c$" "abc" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-complex-not_ol-9" "^a(b)*c$" "abc" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-complex-not_ol-10" "^a(b)*c$" "\nabc\n" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-complex-not_ol-11" "^a(b)*c$" "\na(b)*c\n" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-complex-not_ol-12" "^a(b)*c$" "\nabc\n" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-newline-0" "." "\n" 1 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-newline-1" "." "\n" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-newline-1" "." "\n" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-newline-2" "^abc" "xyz\nabcdef\nxyz" 1 {4 6} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-newline-2" "^abc" "xyz\nabcdef\nxyz" 1 {4 6} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-newline-3" "^abc" "xyz\nxabcdef\nxyz" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-newline-3" "^abc" "xyz\nxabcdef\nxyz" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-newline-4" "z$\nabc" "xyz\nabcdef\nxyz" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-newline-5" "z$\nabc" "xyz\nabcdef\nxyz" 1 {2 6} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-newline-6" "^abc" "abc\nabcdef\nxyz" 1 {4 6} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-newline-6" "^abc" "abc\nabcdef\nxyz" 1 {4 6} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-newline-7" "def$" "xyz\nabcdef\nxyz" 1 {7 9} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-newline-7" "def$" "xyz\nabcdef\nxyz" 1 {7 9} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-newline-8" "def$" "xyz\nxabcdefx\nxyz" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-newline-8" "def$" "xyz\nxabcdefx\nxyz" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-newline-9" "def$\nx" "xyz\nabcdef\nxyz" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-newline-10" "def$\nx" "xyz\nabcdef\nxyz" 1 {7 11} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-complex-newline-2" "^a(b)*c" "xyz\nabcdef\nxyz" 1 {4 6} {5 5} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-complex-newline-3" "^a(b)*c" "xyz\nxabcdef\nxyz" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-complex-newline-4" "z$\na(b)*c" "xyz\nabcdef\nxyz" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-complex-newline-5" "z$\na(b)*c" "xyz\nabcdef\nxyz" 1 {2 6} {5 5} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-complex-newline-6" "^a(b)*c" "abc\nabcdef\nxyz" 1 {4 6} {5 5} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-complex-newline-7" "d(e)f$" "xyz\nabcdef\nxyz" 1 {7 9} {8 8} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-complex-newline-8" "d(e)f$" "xyz\nxabcdefx\nxyz" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-complex-newline-9" "d(e)f$\nx" "xyz\nabcdef\nxyz" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-complex-newline-10" "d(e)f$\nx" "xyz\nabcdef\nxyz" 1 {7 11} {8 8} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-2 -2}  
re-test "posix-check-9" "(abcd){10,11}" "abcd" 0 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
re-test "posix-check-11" "back-tracking oriented stream-of-solution functions" "in the spec, and the back-tracking oriented stream-of-solution functions" 1 {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1} {-1 -1}  
