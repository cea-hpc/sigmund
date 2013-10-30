#!/bin/bash

########################## TEST HELPERS ##################

############################ TEST FUNCTIONS ############################


### test31f : Simple set/get on a acl on a file
SPEED[test31f]=fast
TEST_TYPE[test31f]=dev,admin,prod
NEEDS_ROOT[test31f]=no
function test31f
{
   dir="$TEST_DIR/dir.$$"
   mkdir -p $dir

   rm -fr $dir/file_acl
   touch $dir/file_acl

   setfacl -m user:8989:rw,group:8989:rw $dir/file_acl || error "ERROR while setting acl on a file"

   read_value_user=`getfacl -n $dir/file_acl | grep "user:8989:"`
   read_value_group=`getfacl -n $dir/file_acl | grep "group:8989:"`

   [[ $read_value_user = "user:8989:rw-" ]] || error "ERROR after reading acl. Bad value read"
   [[ $read_value_group = "group:8989:rw-" ]] || error "ERROR after reading acl. Bad value read"
}

### test31d : Simple set/get on a acl on a directory
SPEED[test31d]=fast
TEST_TYPE[test31d]=dev,admin,prod
NEEDS_ROOT[test31d]=no
function test31d
{
   dir="$TEST_DIR/dir.$$"
   mkdir -p $dir

   rm -fr $dir/dir_acl
   mkdir $dir/dir_acl

   setfacl -m user:8989:rw,group:8989:rw $dir/dir_acl || error "ERROR while setting acl on a file"

   read_value_user=`getfacl -n $dir/dir_acl | grep "user:8989:"`
   read_value_group=`getfacl -n $dir/dir_acl | grep "group:8989:"`

   [[ $read_value_user = "user:8989:rw-" ]] || error "ERROR after reading acl. Bad value read"
   [[ $read_value_group = "group:8989:rw-" ]] || error "ERROR after reading acl. Bad value read"
}

### test31s : Simple set/get on a acl on a symlink
SPEED[test31s]=fast
TEST_TYPE[test31s]=dev,admin,prod
NEEDS_ROOT[test31s]=no
function test31s
{
   dir="$TEST_DIR/dir.$$"
   mkdir -p $dir

   rm -fr $dir/file_acl  $dir/symlink_acl
   touch $dir/file_acl
   ln -s $dir/file_acl $dir/symlink_acl

   setfacl -m user:8989:rw,group:8989:rw $dir/symlink_acl || error "ERROR while setting acl on a file"

   read_value_user=`getfacl -n $dir/symlink_acl | grep "user:8989:"`
   read_value_group=`getfacl -n $dir/symlink_acl | grep "group:8989:"`

   [[ $read_value_user = "user:8989:rw-" ]] || error "ERROR after reading acl. Bad value read"
   [[ $read_value_group = "group:8989:rw-" ]] || error "ERROR after reading acl. Bad value read"
}

### test32f: Creates 100 acl and then delete them on a file
SPEED[test32f]=fast
TEST_TYPE[test32f]=dev,admin,prod
NEEDS_ROOT[test32f]=no
function test32f
{ 
   MAX=10
   dir="$TEST_DIR/dir.$$"
   mkdir -p $dir

   rm -fr $dir/file_acl
   touch $dir/file_acl

   for i in `seq 1 $MAX` ; do 
      setfacl -m user:$i:rw  $dir/file_acl || error "ERROR while setting acl on a file"
   done

   # List acl and then delete them one by one
   for i in `seq 1 $MAX` ; do 
     # Do not forget ACL "user::rw-" which exists by default

     count=`getfacl -n $dir/file_acl 2>&1 | grep "user:" | wc -l`
     # in fact this is "$MAX-$i+1+1" (to take care of "user::rw-" which is not taken in account)
     remain=`echo "$MAX-$i+2" | bc`

     [[ $count = $remain ]] || error "ERROR bad count when deleting acl"
     setfacl -x  user:$i  $dir/file_acl 
     echo "$i acl deleted, $remain remains"
   done

   # Make sure no acl remain 
   count=`getfacl -n  $dir/file_acl | grep "user:" | wc -l`
   (( $count > 1 )) && error "ERROR acl remain when they are all supposed to be deleted"
}

### test32d: Creates 100 acl and then delete them on a directory
SPEED[test32d]=fast
TEST_TYPE[test32d]=dev,admin,prod
NEEDS_ROOT[test32d]=no
function test32d
{ 
   MAX=10
   dir="$TEST_DIR/dir.$$"
   mkdir -p $dir

   rm -fr $dir/dir_acl
   mkdir $dir/dir_acl

   for i in `seq 1 $MAX` ; do 
      setfacl -m user:$i:rw  $dir/dir_acl || error "ERROR while setting acl on a directory"
   done

   # List acl and then delete them one by one
   for i in `seq 1 $MAX` ; do 
     count=`getfacl -n  $dir/dir_acl 2>&1 | grep "user:" | wc -l`
     # in fact this is "$MAX-$i+1+1" (to take care of "user::rw-" which is not taken in account)
     remain=`echo "$MAX-$i+2" | bc`

     [[ $count = $remain ]] || error "ERROR bad count when deleting acl"
     setfacl -x  user:$i  $dir/dir_acl 
     echo "$i acl deleted, $remain remains"
   done

   # Make sure no acl remain 
   count=`getfacl -n  $dir/dir_acl | grep "user:" | wc -l`
   (( $count > 1 )) && error "ERROR acl remain when they are all supposed to be deleted"
}

### test32s: Creates 100 acl and then delete them on a directory
SPEED[test32s]=fast
TEST_TYPE[test32s]=dev,admin,prod
NEEDS_ROOT[test32s]=no
function test32s
{ 
   MAX=10
   dir="$TEST_DIR/dir.$$"
   mkdir -p $dir

   rm -fr $dir/file_acl  $dir/symlink_acl
   touch $dir/file_acl
   ln -s $dir/file_acl $dir/symlink_acl

   for i in `seq 1 $MAX` ; do 
      setfacl -m user:$i:rw  $dir/symlink_acl || error "ERROR while setting acl on a symlink"
   done

   # List acl and then delete them one by one
   for i in `seq 1 $MAX` ; do 
     count=`getfacl -n  $dir/symlink_acl 2>&1 | grep "user:" | wc -l`
     # in fact this is "$MAX-$i+1+1" (to take care of "user::rw-" which is not taken in account)
     remain=`echo "$MAX-$i+2" | bc`

     [[ $count = $remain ]] || error "ERROR bad count when deleting acl"
     setfacl -x  user:$i  $dir/symlink_acl 
     echo "$i acl deleted, $remain remains"
   done

   # Make sure no acl remain 
   count=`getfacl -n  $dir/symlink_acl | grep "user:" | wc -l`
   (( $count > 1 )) && error "ERROR acl remain when they are all supposed to be deleted"
}

### test33f : Set a value several times then check value
SPEED[test33f]=fast
TEST_TYPE[test33f]=dev,admin,prod
NEEDS_ROOT[test33f]=no
function test33f
{
   MAX=10
   dir="$TEST_DIR/dir.$$"
   mkdir -p $dir

   rm -fr $dir/file_acl
   touch $dir/file_acl

   for i in `seq 1 $MAX` ; do
      setfacl -m user:8989:rw  $dir/file_acl || error "ERROR while setting acl on a file"
   done

   read_value=`getfacl $dir/file_acl | grep "user:8989:"`

   [[ $read_value = "user:8989:rw-" ]] || error "ERROR after reading acl. Bad value read"
}

### test33d :  Set a value several times then check value
SPEED[test33d]=fast
TEST_TYPE[test33d]=dev,admin,prod
NEEDS_ROOT[test33d]=no
function test33d
{
   MAX=10
   dir="$TEST_DIR/dir.$$"
   mkdir -p $dir

   rm -fr $dir/dir_acl
   mkdir $dir/dir_acl

   for i in `seq 1 $MAX` ; do
      setfacl -m user:8989:rw  $dir/dir_acl || error "ERROR while setting acl on a file"
   done 

   read_value=`getfacl $dir/dir_acl | grep "user:8989:"`

   [[ $read_value = "user:8989:rw-" ]] || error "ERROR after reading acl. Bad value read"
}

### test33s : Set a value several times then check value
SPEED[test33s]=fast
TEST_TYPE[test33s]=dev,admin,prod
NEEDS_ROOT[test33s]=no
function test33s
{
   MAX=10
   dir="$TEST_DIR/dir.$$"
   mkdir -p $dir

   rm -fr $dir/file_acl  $dir/symlink_acl
   touch $dir/file_acl
   ln -s $dir/file_acl $dir/symlink_acl

   for i in `seq 1 $MAX` ; do
      setfacl -m user:8989:rw  $dir/symlink_acl || error "ERROR while setting acl on a file"
   done 

   read_value=`getfacl $dir/symlink_acl | grep "user:8989:"`

   [[ $read_value = "user:8989:rw-" ]] || error "ERROR after reading acl. Bad value read"
}


######################## DEFINE TEST LIST HERE ####################

function run_acl
{
  run_test test31f  "ACL: simple acl set/get : file        "
  run_test test31d  "ACL: simple acl set/get : directory   "
  run_test test31s  "ACL: simple acl set/get : symlink     "
  run_test test32f  "ACL: acl creation/deletion : file     "
  run_test test32d  "ACL: acl creation/deletion : directory"
  run_test test32s  "ACL: acl creation/deletion : symlink  "
  run_test test33f  "ACL: mulitple sets + 1 check : file     "
  run_test test33d  "ACL: mulitple sets + 1 check : directory"
  run_test test33s  "ACL: mulitple sets + 1 check : symlink  "
}
