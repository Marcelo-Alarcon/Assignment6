.class public TestIf
.super java/lang/Object

.field private static _sysin Ljava/util/Scanner;
.field private static i I
.field private static j I
.field private static k I
.field private static p Z
.field private static q Z
.field private static str Ljava/lang/String;
.field private static txt Ljava/lang/String;
.field private static x F
.field private static y F

;
; Runtime input scanner
;
.method static <clinit>()V

	new	java/util/Scanner
	dup
	getstatic	java/lang/System/in Ljava/io/InputStream;
	invokespecial	java/util/Scanner/<init>(Ljava/io/InputStream;)V
	putstatic	TestIf/_sysin Ljava/util/Scanner;
	return

.limit locals 0
.limit stack 3
.end method

;
; Main class constructor
;
.method public <init>()V
.var 0 is this LTestIf;

	aload_0
	invokespecial	java/lang/Object/<init>()V
	return

.limit locals 1
.limit stack 1
.end method

;
; MAIN
;
.method public static main([Ljava/lang/String;)V
.var 0 is args [Ljava/lang/String;
.var 1 is _start Ljava/time/Instant;
.var 2 is _end Ljava/time/Instant;
.var 3 is _elapsed J

	invokestatic	java/time/Instant/now()Ljava/time/Instant;
	astore_1

;
; 010 i:=1
;
	iconst_1
	putstatic	TestIf/i I
;
; 011 j:=2
;
	iconst_2
	putstatic	TestIf/j I
;
; 013 IFi=jTHENx:=3.14ELSEx:=-5
;
	getstatic	TestIf/i I
	getstatic	TestIf/j I
	if_icmpeq	L003
	iconst_0
	goto	L004
L003:
	iconst_1
L004:
	ifeq	L002
;
; 013 x:=3.14
;
	ldc	3.14
	putstatic	TestIf/x F
	goto	L001
L002:
;
; 014 x:=-5
;
	iconst_5
	ineg
	i2f
	putstatic	TestIf/x F
L001:
;
; 016 IFi<>jTHENy:=3.14ELSEy:=-5
;
	getstatic	TestIf/i I
	getstatic	TestIf/j I
	if_icmpne	L007
	iconst_0
	goto	L008
L007:
	iconst_1
L008:
	ifeq	L006
;
; 016 y:=3.14
;
	ldc	3.14
	putstatic	TestIf/y F
	goto	L005
L006:
;
; 017 y:=-5
;
	iconst_5
	ineg
	i2f
	putstatic	TestIf/y F
L005:
;
; 019 writeln('i = ',i,', j = ',j,', x = ',x:5:2,', y = ',y:5:2)
;
	getstatic	java/lang/System/out Ljava/io/PrintStream;
	ldc	"i = %d, j = %d, x = %5.2f, y = %5.2f\n"
	iconst_4
	anewarray	java/lang/Object
	dup
	iconst_0
	getstatic	TestIf/i I
	invokestatic	java/lang/Integer/valueOf(I)Ljava/lang/Integer;
	aastore
	dup
	iconst_1
	getstatic	TestIf/j I
	invokestatic	java/lang/Integer/valueOf(I)Ljava/lang/Integer;
	aastore
	dup
	iconst_2
	getstatic	TestIf/x F
	invokestatic	java/lang/Float/valueOf(F)Ljava/lang/Float;
	aastore
	dup
	iconst_3
	getstatic	TestIf/y F
	invokestatic	java/lang/Float/valueOf(F)Ljava/lang/Float;
	aastore
	invokevirtual	java/io/PrintStream/printf(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;
	pop
;
; 021 IFi=jTHENBEGINx:=-7ENDELSEBEGINx:=8;END
;
	getstatic	TestIf/i I
	getstatic	TestIf/j I
	if_icmpeq	L011
	iconst_0
	goto	L012
L011:
	iconst_1
L012:
	ifeq	L010
;
; 022 x:=-7
;
	bipush	7
	ineg
	i2f
	putstatic	TestIf/x F
	goto	L009
L010:
;
; 025 x:=8
;
	bipush	8
	i2f
	putstatic	TestIf/x F
L009:
;
; 028 IFi<>jTHENBEGINy:=14ENDELSEBEGINy:=-2;END
;
	getstatic	TestIf/i I
	getstatic	TestIf/j I
	if_icmpne	L015
	iconst_0
	goto	L016
L015:
	iconst_1
L016:
	ifeq	L014
;
; 029 y:=14
;
	bipush	14
	i2f
	putstatic	TestIf/y F
	goto	L013
L014:
;
; 032 y:=-2
;
	iconst_2
	ineg
	i2f
	putstatic	TestIf/y F
L013:
;
; 035 writeln('i = ',i,', j = ',j,', x = ',x:5:2,', y = ',y:5:2)
;
	getstatic	java/lang/System/out Ljava/io/PrintStream;
	ldc	"i = %d, j = %d, x = %5.2f, y = %5.2f\n"
	iconst_4
	anewarray	java/lang/Object
	dup
	iconst_0
	getstatic	TestIf/i I
	invokestatic	java/lang/Integer/valueOf(I)Ljava/lang/Integer;
	aastore
	dup
	iconst_1
	getstatic	TestIf/j I
	invokestatic	java/lang/Integer/valueOf(I)Ljava/lang/Integer;
	aastore
	dup
	iconst_2
	getstatic	TestIf/x F
	invokestatic	java/lang/Float/valueOf(F)Ljava/lang/Float;
	aastore
	dup
	iconst_3
	getstatic	TestIf/y F
	invokestatic	java/lang/Float/valueOf(F)Ljava/lang/Float;
	aastore
	invokevirtual	java/io/PrintStream/printf(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;
	pop
;
; 037 p:=i=j
;
	getstatic	TestIf/i I
	getstatic	TestIf/j I
	if_icmpeq	L017
	iconst_0
	goto	L018
L017:
	iconst_1
L018:
	putstatic	TestIf/p Z
;
; 038 IFpTHENx:=55.55ELSEBEGINx:=77.77;y:=88.88;END
;
	getstatic	TestIf/p Z
	ifeq	L020
;
; 038 x:=55.55
;
	ldc	55.55
	putstatic	TestIf/x F
	goto	L019
L020:
;
; 040 x:=77.77
;
	ldc	77.77
	putstatic	TestIf/x F
;
; 041 y:=88.88
;
	ldc	88.88
	putstatic	TestIf/y F
L019:
;
; 044 writeln('i = ',i,', j = ',j,', x = ',x:5:2,', y = ',y:5:2)
;
	getstatic	java/lang/System/out Ljava/io/PrintStream;
	ldc	"i = %d, j = %d, x = %5.2f, y = %5.2f\n"
	iconst_4
	anewarray	java/lang/Object
	dup
	iconst_0
	getstatic	TestIf/i I
	invokestatic	java/lang/Integer/valueOf(I)Ljava/lang/Integer;
	aastore
	dup
	iconst_1
	getstatic	TestIf/j I
	invokestatic	java/lang/Integer/valueOf(I)Ljava/lang/Integer;
	aastore
	dup
	iconst_2
	getstatic	TestIf/x F
	invokestatic	java/lang/Float/valueOf(F)Ljava/lang/Float;
	aastore
	dup
	iconst_3
	getstatic	TestIf/y F
	invokestatic	java/lang/Float/valueOf(F)Ljava/lang/Float;
	aastore
	invokevirtual	java/io/PrintStream/printf(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;
	pop
;
; 046 q:=i<=j
;
	getstatic	TestIf/i I
	getstatic	TestIf/j I
	if_icmple	L021
	iconst_0
	goto	L022
L021:
	iconst_1
L022:
	putstatic	TestIf/q Z
;
; 047 k:=10
;
	bipush	10
	putstatic	TestIf/k I
;
; 049 ifptheni:=33elseifnotqtheni:=44elseifi=jtheni:=55elsei:=6
;
	getstatic	TestIf/p Z
	ifeq	L024
;
; 049 i:=33
;
	bipush	33
	putstatic	TestIf/i I
	goto	L023
L024:
;
; 049 ifnotqtheni:=44elseifi=jtheni:=55elsei:=6
;
	getstatic	TestIf/q Z
	iconst_1
	ixor
	ifeq	L026
;
; 049 i:=44
;
	bipush	44
	putstatic	TestIf/i I
	goto	L025
L026:
;
; 049 ifi=jtheni:=55elsei:=6
;
	getstatic	TestIf/i I
	getstatic	TestIf/j I
	if_icmpeq	L029
	iconst_0
	goto	L030
L029:
	iconst_1
L030:
	ifeq	L028
;
; 049 i:=55
;
	bipush	55
	putstatic	TestIf/i I
	goto	L027
L028:
;
; 049 i:=6
;
	bipush	6
	putstatic	TestIf/i I
L027:
L025:
L023:
;
; 050 ifnotpthenifqthenj:=9elsej:=-9
;
	getstatic	TestIf/p Z
	iconst_1
	ixor
	ifeq	L032
;
; 050 ifqthenj:=9elsej:=-9
;
	getstatic	TestIf/q Z
	ifeq	L034
;
; 050 j:=9
;
	bipush	9
	putstatic	TestIf/j I
	goto	L033
L034:
;
; 050 j:=-9
;
	bipush	9
	ineg
	putstatic	TestIf/j I
L033:
L032:
;
; 051 ifpthenifqthenk:=11elsek:=12
;
	getstatic	TestIf/p Z
	ifeq	L036
;
; 051 ifqthenk:=11elsek:=12
;
	getstatic	TestIf/q Z
	ifeq	L038
;
; 051 k:=11
;
	bipush	11
	putstatic	TestIf/k I
	goto	L037
L038:
;
; 051 k:=12
;
	bipush	12
	putstatic	TestIf/k I
L037:
L036:
;
; 053 writeln('i = ',i,', j = ',j,', x = ',x:5:2,', y = ',y:5:2)
;
	getstatic	java/lang/System/out Ljava/io/PrintStream;
	ldc	"i = %d, j = %d, x = %5.2f, y = %5.2f\n"
	iconst_4
	anewarray	java/lang/Object
	dup
	iconst_0
	getstatic	TestIf/i I
	invokestatic	java/lang/Integer/valueOf(I)Ljava/lang/Integer;
	aastore
	dup
	iconst_1
	getstatic	TestIf/j I
	invokestatic	java/lang/Integer/valueOf(I)Ljava/lang/Integer;
	aastore
	dup
	iconst_2
	getstatic	TestIf/x F
	invokestatic	java/lang/Float/valueOf(F)Ljava/lang/Float;
	aastore
	dup
	iconst_3
	getstatic	TestIf/y F
	invokestatic	java/lang/Float/valueOf(F)Ljava/lang/Float;
	aastore
	invokevirtual	java/io/PrintStream/printf(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;
	pop
;
; 054 writeln('k = ',k)
;
	getstatic	java/lang/System/out Ljava/io/PrintStream;
	ldc	"k = %d\n"
	iconst_1
	anewarray	java/lang/Object
	dup
	iconst_0
	getstatic	TestIf/k I
	invokestatic	java/lang/Integer/valueOf(I)Ljava/lang/Integer;
	aastore
	invokevirtual	java/io/PrintStream/printf(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;
	pop
;
; 056 writeln
;
	getstatic	java/lang/System/out Ljava/io/PrintStream;
	invokevirtual	java/io/PrintStream.println()V
;
; 057 ifnotpandqand(i<>j)and(x<y)thenwrite('Good-bye')
;
	getstatic	TestIf/p Z
	iconst_1
	ixor
	getstatic	TestIf/q Z
	iand
	getstatic	TestIf/i I
	getstatic	TestIf/j I
	if_icmpne	L041
	iconst_0
	goto	L042
L041:
	iconst_1
L042:
	iand
	getstatic	TestIf/x F
	getstatic	TestIf/y F
	fcmpg
	iflt	L043
	iconst_0
	goto	L044
L043:
	iconst_1
L044:
	iand
	ifeq	L040
;
; 057 write('Good-bye')
;
	getstatic	java/lang/System/out Ljava/io/PrintStream;
	ldc	"Good-bye"
	invokevirtual	java/io/PrintStream/print(Ljava/lang/String;)V
L040:
;
; 058 ifnotpthenifqthenifi<>jthenifx<ythenwriteln(', world!')
;
	getstatic	TestIf/p Z
	iconst_1
	ixor
	ifeq	L046
;
; 058 ifqthenifi<>jthenifx<ythenwriteln(', world!')
;
	getstatic	TestIf/q Z
	ifeq	L048
;
; 058 ifi<>jthenifx<ythenwriteln(', world!')
;
	getstatic	TestIf/i I
	getstatic	TestIf/j I
	if_icmpne	L051
	iconst_0
	goto	L052
L051:
	iconst_1
L052:
	ifeq	L050
;
; 058 ifx<ythenwriteln(', world!')
;
	getstatic	TestIf/x F
	getstatic	TestIf/y F
	fcmpg
	iflt	L055
	iconst_0
	goto	L056
L055:
	iconst_1
L056:
	ifeq	L054
;
; 058 writeln(', world!')
;
	getstatic	java/lang/System/out Ljava/io/PrintStream;
	ldc	", world!\n"
	invokevirtual	java/io/PrintStream/print(Ljava/lang/String;)V
L054:
L050:
L048:
L046:

	invokestatic	java/time/Instant/now()Ljava/time/Instant;
	astore_2
	aload_1
	aload_2
	invokestatic	java/time/Duration/between(Ljava/time/temporal/Temporal;Ljava/time/temporal/Temporal;)Ljava/time/Duration;
	invokevirtual	java/time/Duration/toMillis()J
	lstore_3
	getstatic	java/lang/System/out Ljava/io/PrintStream;
	ldc	"\n[%,d milliseconds execution time.]\n"
	iconst_1
	anewarray	java/lang/Object
	dup
	iconst_0
	lload_3
	invokestatic	java/lang/Long/valueOf(J)Ljava/lang/Long;
	aastore
	invokevirtual	java/io/PrintStream/printf(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;
	pop

	return

.limit locals 6
.limit stack 7
.end method
