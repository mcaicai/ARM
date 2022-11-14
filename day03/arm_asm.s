
.text						@表示当前为代码段
.global _start	@将_start定义成全局符号

_start: 				@汇编的入口

		MOV R1, #1		@汇编指令
		MOV R2, #2
		ADD R1, R2, R3

stop:
		B stop

.end						@汇编结束
