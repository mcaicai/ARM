
.text						@��ʾ��ǰΪ�����
.global _start	@��_start�����ȫ�ַ���

_start: 				@�������

		MOV R1, #1		@���ָ��
		MOV R2, #2
		ADD R1, R2, R3

stop:
		B stop

.end						@������
