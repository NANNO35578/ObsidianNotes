
*开始流程*
1. Client_A发起语音通话请求, 发送*语音通话请求*
	:RiNumber1: call->hide; callDetail->show;
	:RiNumber2: callDetail->text: "Calling"
2. 服务器转发发起语音通话请求
3. Client_B接收到语音通话请求
4. C_B同意发起语音通话请求, 发送*语音通话请求回复*
5. 服务器转发语音通话回复
6. C_A收到同意回复
	:RiNumber1: callDetail->text: "On The Phone"
7. 开始通话

*挂断流程*
1. Client_B发起*挂断语音通话请求*
2. 服务器转发发起挂断音通话请求
3. Client_A接收到挂断语音通话请求
4. 断开通话






















































