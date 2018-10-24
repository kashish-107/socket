all : aws

aws : aws.c
      gcc -o aws aws.c


