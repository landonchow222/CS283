#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Starting client without starting server results error" {
    run ./dsh -c <<EOF                
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="Theserverisdown.startclient:Connectionrefusedsocketclientmode:addr:127.0.0.1:1234cmdloopreturned-52"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Stop server" {
    # Start dsh -s in the background
    ./dsh -s &

    run ./dsh -c <<EOF                
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh3>clientrequestedservertostop,stopping...cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]

}

@test "exit client" {
    # Start dsh -s in the background
    ./dsh -s &
    pid=$!

    run ./dsh -c <<EOF                
exit
EOF

    kill $pid

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh3>clientexited:gettingnextconnection...cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

# @test "exit client" {
#     # Start dsh -s in the background
#     ./dsh -s &
#     pid=$!

#     run ./dsh -c <<EOF                
# exit
# EOF

#     kill $pid

#     # Strip all whitespace (spaces, tabs, newlines) from the output
#     stripped_output=$(echo "$output" | tr -d '[:space:]')

#     # Expected output with all whitespace removed for easier matching
#     expected_output="socketclientmode:addr:127.0.0.1:1234dsh3>clientexited:gettingnextconnection...cmdloopreturned0"

#     # These echo commands will help with debugging and will only print
#     #if the test fails
#     echo "Captured stdout:" 
#     echo "Output: $output"
#     echo "Exit Status: $status"
#     echo "${stripped_output} -> ${expected_output}"

#     # Check exact match
#     [ "$stripped_output" = "$expected_output" ]

#     # Assertions
#     [ "$status" -eq 0 ]
# }

# @test "exit client" {
#     # Start dsh -s in the background
#     ./dsh -s &
#     pid=$!

#     run ./dsh -c <<EOF                
# exit
# EOF

#     kill $pid

#     # Strip all whitespace (spaces, tabs, newlines) from the output
#     stripped_output=$(echo "$output" | tr -d '[:space:]')

#     # Expected output with all whitespace removed for easier matching
#     expected_output="socketclientmode:addr:127.0.0.1:1234dsh3>clientexited:gettingnextconnection...cmdloopreturned0"

#     # These echo commands will help with debugging and will only print
#     #if the test fails
#     echo "Captured stdout:" 
#     echo "Output: $output"
#     echo "Exit Status: $status"
#     echo "${stripped_output} -> ${expected_output}"

#     # Check exact match
#     [ "$stripped_output" = "$expected_output" ]

#     # Assertions
#     [ "$status" -eq 0 ]
# }

# @test "exit client" {
#     # Start dsh -s in the background
#     ./dsh -s &
#     pid=$!

#     run ./dsh -c <<EOF                
# exit
# EOF

#     kill $pid

#     # Strip all whitespace (spaces, tabs, newlines) from the output
#     stripped_output=$(echo "$output" | tr -d '[:space:]')

#     # Expected output with all whitespace removed for easier matching
#     expected_output="socketclientmode:addr:127.0.0.1:1234dsh3>clientexited:gettingnextconnection...cmdloopreturned0"

#     # These echo commands will help with debugging and will only print
#     #if the test fails
#     echo "Captured stdout:" 
#     echo "Output: $output"
#     echo "Exit Status: $status"
#     echo "${stripped_output} -> ${expected_output}"

#     # Check exact match
#     [ "$stripped_output" = "$expected_output" ]

#     # Assertions
#     [ "$status" -eq 0 ]
# }

# @test "exit client" {
#     # Start dsh -s in the background
#     ./dsh -s &
#     pid=$!

#     run ./dsh -c <<EOF                
# exit
# EOF

#     kill $pid

#     # Strip all whitespace (spaces, tabs, newlines) from the output
#     stripped_output=$(echo "$output" | tr -d '[:space:]')

#     # Expected output with all whitespace removed for easier matching
#     expected_output="socketclientmode:addr:127.0.0.1:1234dsh3>clientexited:gettingnextconnection...cmdloopreturned0"

#     # These echo commands will help with debugging and will only print
#     #if the test fails
#     echo "Captured stdout:" 
#     echo "Output: $output"
#     echo "Exit Status: $status"
#     echo "${stripped_output} -> ${expected_output}"

#     # Check exact match
#     [ "$stripped_output" = "$expected_output" ]

#     # Assertions
#     [ "$status" -eq 0 ]
# }