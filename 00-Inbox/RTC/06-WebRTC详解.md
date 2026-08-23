# 第六章：WebRTC详解

## 6.1 WebRTC概述

### 6.1.1 什么是WebRTC

**WebRTC（Web Real-Time Communication）** 是一个开源项目，旨在使浏览器和移动应用能够进行实时音视频通信。

#### 特点
- **开源免费**：Google主导，开源项目
- **跨平台**：支持Web、iOS、Android
- **无需插件**：浏览器原生支持
- **P2P通信**：支持点对点直连
- **标准协议**：基于标准协议（RTP、ICE等）

### 6.1.2 WebRTC架构

```
┌─────────────────────────────────────┐
│      Web应用（JavaScript API）      │
├─────────────────────────────────────┤
│      WebRTC API（浏览器层）         │
├─────────────────────────────────────┤
│   信令层（需要自己实现）            │
├─────────────────────────────────────┤
│   媒体引擎（音视频处理）            │
├─────────────────────────────────────┤
│   传输层（RTP/RTCP、ICE）           │
└─────────────────────────────────────┘
```

### 6.1.3 浏览器支持

- **Chrome**：完整支持
- **Firefox**：完整支持
- **Safari**：支持（iOS 11+）
- **Edge**：完整支持（基于Chromium）

## 6.2 核心API

### 6.2.1 getUserMedia

获取本地音视频流。

```javascript
// 获取音视频流
navigator.mediaDevices.getUserMedia({
  audio: true,  // 音频
  video: true   // 视频
})
.then(stream => {
  // 使用stream
  videoElement.srcObject = stream;
})
.catch(error => {
  console.error('获取媒体失败:', error);
});
```

#### 约束参数

```javascript
const constraints = {
  audio: {
    echoCancellation: true,    // 回声消除
    noiseSuppression: true,     // 降噪
    autoGainControl: true,      // 自动增益
    sampleRate: 48000          // 采样率
  },
  video: {
    width: { ideal: 1280 },     // 宽度
    height: { ideal: 720 },     // 高度
    frameRate: { ideal: 30 },   // 帧率
    facingMode: 'user'          // 前置/后置摄像头
  }
};
```

### 6.2.2 RTCPeerConnection

建立P2P连接的核心API。

```javascript
// 创建PeerConnection
const pc = new RTCPeerConnection({
  iceServers: [
    { urls: 'stun:stun.l.google.com:19302' },
    {
      urls: 'turn:turnserver.com:3478',
      username: 'user',
      credential: 'pass'
    }
  ]
});

// 添加本地流
stream.getTracks().forEach(track => {
  pc.addTrack(track, stream);
});

// 处理远程流
pc.ontrack = (event) => {
  remoteVideo.srcObject = event.streams[0];
};

// ICE候选处理
pc.onicecandidate = (event) => {
  if (event.candidate) {
    // 通过信令服务器发送给对端
    sendCandidate(event.candidate);
  }
};
```

### 6.2.3 连接建立流程

```javascript
// 发起端（Offer）
async function createOffer() {
  const offer = await pc.createOffer();
  await pc.setLocalDescription(offer);
  // 通过信令服务器发送offer
  sendOffer(offer);
}

// 接收端（Answer）
async function createAnswer(offer) {
  await pc.setRemoteDescription(offer);
  const answer = await pc.createAnswer();
  await pc.setLocalDescription(answer);
  // 通过信令服务器发送answer
  sendAnswer(answer);
}

// 设置远程描述
async function setRemoteDescription(answer) {
  await pc.setRemoteDescription(answer);
}
```

### 6.2.4 数据通道（DataChannel）

用于传输非音视频数据。

```javascript
// 创建数据通道
const dataChannel = pc.createDataChannel('chat', {
  ordered: true  // 保证顺序
});

// 发送数据
dataChannel.send('Hello WebRTC!');

// 接收数据
dataChannel.onmessage = (event) => {
  console.log('收到数据:', event.data);
};

// 处理对端数据通道
pc.ondatachannel = (event) => {
  const channel = event.channel;
  channel.onmessage = (event) => {
    console.log('收到数据:', event.data);
  };
};
```

## 6.3 信令服务器

### 6.3.1 为什么需要信令服务器

WebRTC本身不提供信令机制，需要自己实现：
- **SDP交换**：交换媒体描述信息
- **ICE候选交换**：交换网络连接信息
- **房间管理**：管理用户和房间

### 6.3.2 WebSocket信令服务器

#### 客户端代码

```javascript
const ws = new WebSocket('ws://signaling-server.com');

// 发送信令
function sendSignaling(data) {
  ws.send(JSON.stringify(data));
}

// 接收信令
ws.onmessage = (event) => {
  const data = JSON.parse(event.data);
  
  switch(data.type) {
    case 'offer':
      handleOffer(data.offer);
      break;
    case 'answer':
      handleAnswer(data.answer);
      break;
    case 'candidate':
      handleCandidate(data.candidate);
      break;
  }
};
```

#### 服务器端代码（Node.js示例）

```javascript
const WebSocket = require('ws');
const wss = new WebSocket.Server({ port: 8080 });

const rooms = new Map();

wss.on('connection', (ws) => {
  ws.on('message', (message) => {
    const data = JSON.parse(message);
    
    switch(data.type) {
      case 'join':
        joinRoom(ws, data.roomId);
        break;
      case 'offer':
      case 'answer':
      case 'candidate':
        broadcastToOthers(ws, data);
        break;
    }
  });
});

function joinRoom(ws, roomId) {
  if (!rooms.has(roomId)) {
    rooms.set(roomId, []);
  }
  rooms.get(roomId).push(ws);
  
  // 通知其他用户
  rooms.get(roomId).forEach(client => {
    if (client !== ws && client.readyState === WebSocket.OPEN) {
      client.send(JSON.stringify({
        type: 'user-joined',
        userId: ws.id
      }));
    }
  });
}
```

## 6.4 SDP（Session Description Protocol）

### 6.4.1 SDP概述

SDP用于描述媒体会话信息，包括：
- 媒体类型（音频/视频）
- 编码格式
- 网络信息
- 传输信息

### 6.4.2 SDP格式

```
v=0
o=- 123456789 2 IN IP4 127.0.0.1
s=-
t=0 0
a=group:BUNDLE 0 1
a=msid-semantic: WMS
m=audio 9 UDP/TLS/RTP/SAVPF 111
c=IN IP4 0.0.0.0
a=rtcp:9 IN IP4 0.0.0.0
a=ice-ufrag:xxxx
a=ice-pwd:xxxx
a=fingerprint:sha-256 xxxx
a=setup:actpass
a=mid:0
a=recvonly
a=rtpmap:111 opus/48000/2
m=video 9 UDP/TLS/RTP/SAVPF 96
c=IN IP4 0.0.0.0
a=rtcp:9 IN IP4 0.0.0.0
a=ice-ufrag:xxxx
a=ice-pwd:xxxx
a=fingerprint:sha-256 xxxx
a=setup:actpass
a=mid:1
a=recvonly
a=rtpmap:96 VP8/90000
```

### 6.4.3 SDP关键字段

- **v**：版本
- **o**：会话标识
- **m**：媒体描述（音频/视频）
- **c**：连接信息
- **a**：属性
  - **ice-ufrag/ice-pwd**：ICE认证信息
  - **fingerprint**：DTLS指纹
  - **rtpmap**：RTP负载类型映射
  - **sendonly/recvonly/sendrecv**：媒体方向

### 6.4.4 SDP修改

可以修改SDP来优化连接：

```javascript
// 修改SDP
const modifiedSdp = sdp.replace(
  /a=fmtp:111/g,
  'a=fmtp:111 minptime=10;useinbandfec=1'
);

// 设置修改后的SDP
await pc.setLocalDescription({
  type: pc.localDescription.type,
  sdp: modifiedSdp
});
```

## 6.5 ICE连接建立

### 6.5.1 ICE流程

1. **收集候选（Gathering）**
   - 获取本地IP
   - 通过STUN获取公网IP
   - 通过TURN获取中继地址

2. **交换候选（Exchange）**
   - 通过信令服务器交换候选

3. **连接检查（Connectivity Check）**
   - 尝试建立连接
   - 选择最佳路径

4. **连接建立（Connection Established）**
   - 连接成功
   - 开始传输数据

### 6.5.2 ICE状态

```javascript
pc.oniceconnectionstatechange = () => {
  console.log('ICE状态:', pc.iceConnectionState);
  // new, checking, connected, completed, failed, disconnected, closed
};
```

### 6.5.3 连接状态

```javascript
pc.onconnectionstatechange = () => {
  console.log('连接状态:', pc.connectionState);
  // new, connecting, connected, disconnected, failed, closed
};
```

## 6.6 媒体流控制

### 6.6.1 获取和设置轨道

```javascript
// 获取轨道
const audioTrack = stream.getAudioTracks()[0];
const videoTrack = stream.getVideoTracks()[0];

// 启用/禁用轨道
audioTrack.enabled = true;
videoTrack.enabled = false;

// 停止轨道
audioTrack.stop();
videoTrack.stop();
```

### 6.6.2 切换摄像头

```javascript
async function switchCamera() {
  const videoTrack = stream.getVideoTracks()[0];
  const constraints = {
    video: {
      facingMode: videoTrack.getSettings().facingMode === 'user' 
        ? 'environment' 
        : 'user'
    }
  };
  
  const newStream = await navigator.mediaDevices.getUserMedia(constraints);
  const newTrack = newStream.getVideoTracks()[0];
  
  // 替换轨道
  const sender = pc.getSenders().find(s => 
    s.track && s.track.kind === 'video'
  );
  await sender.replaceTrack(newTrack);
  
  // 停止旧轨道
  videoTrack.stop();
}
```

### 6.6.3 屏幕共享

```javascript
// 获取屏幕共享
const screenStream = await navigator.mediaDevices.getDisplayMedia({
  video: true,
  audio: true
});

// 添加到PeerConnection
screenStream.getTracks().forEach(track => {
  pc.addTrack(track, screenStream);
});
```

## 6.7 统计信息

### 6.7.1 获取统计信息

```javascript
// 获取统计信息
const stats = await pc.getStats();

stats.forEach(report => {
  if (report.type === 'outbound-rtp') {
    console.log('发送字节数:', report.bytesSent);
    console.log('发送包数:', report.packetsSent);
    console.log('丢包数:', report.packetsLost);
  }
  
  if (report.type === 'inbound-rtp') {
    console.log('接收字节数:', report.bytesReceived);
    console.log('接收包数:', report.packetsReceived);
    console.log('抖动:', report.jitter);
  }
});
```

### 6.7.2 监控网络质量

```javascript
setInterval(async () => {
  const stats = await pc.getStats();
  
  // 计算丢包率
  const inboundStats = Array.from(stats.values())
    .find(s => s.type === 'inbound-rtp');
  
  if (inboundStats) {
    const packetLoss = inboundStats.packetsLost / 
      (inboundStats.packetsReceived + inboundStats.packetsLost);
    console.log('丢包率:', packetLoss);
  }
}, 1000);
```

## 6.8 常见问题

### 6.8.1 无法建立连接

- **检查ICE服务器配置**
- **检查防火墙设置**
- **检查NAT类型**
- **使用TURN服务器**

### 6.8.2 音视频不同步

- **检查时间戳**
- **调整播放缓冲**
- **使用同步机制**

### 6.8.3 性能问题

- **降低分辨率**
- **降低帧率**
- **使用硬件编码**
- **优化编码参数**

## 6.9 实践建议

1. **理解连接流程**：掌握SDP和ICE交换过程
2. **实现信令服务器**：使用WebSocket实现信令
3. **处理错误情况**：网络错误、设备错误等
4. **监控连接状态**：实时监控连接质量
5. **优化性能**：根据网络状况调整参数

## 6.10 思考题

1. WebRTC为什么需要信令服务器？
2. SDP在WebRTC中起什么作用？
3. ICE连接建立的过程是什么？
4. 如何实现屏幕共享功能？
5. 如何监控WebRTC连接质量？

---

**上一章**：[第五章：实时通信架构](./05-实时通信架构.md)  
**下一章**：[第七章：音视频处理](./07-音视频处理.md)

