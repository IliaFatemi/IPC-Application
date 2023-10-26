# Assignment 2 Instructions

## 1. Compile program
<b>When compiling, an executable called `s-talk` will be created.</b><br>

Run the following to compile:
```
make all
```

## 2. Run program
>[!IMPORTANT]
><b>Note: Both machines must be on the same network in order to connect. The two machines IPV4 addresses must be on the same network subnet.</b>

The program takes takes <b>4 arguments</b> in the following format:
<b>
```
./s-talk [my port number] [remote machine name] [remote port number]
```

### On computer_A run
```
./s-talk 6060 computer_A 6001
```

### On computer_B run
```
./s-talk 6001 computer_B 6060
```
</b>

## 3. Start chatting 
When chatting, the sender and receiver are color coded to differentiate the sender from the receiver.<br>

>[!NOTE]
>sender is indicated as <b>green.</b><br>
>receiver is indicated as <b>blue.</b><br>
