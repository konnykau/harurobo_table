# 春ロボこいのぼり用　把持と昇降

予備

ビルドは通した

# 使い方

## サーボ把持

doll_holder.hppのなか

二値のみ取る。open ccrとclose ccrはうまく決めて

三個のサーボしか使えない。

```c++
servo_holder Servo_holder(0x300);//コンストラクタの引数はCAN_ID YUKICHI先輩のサーボ基板使うなら0x300

Servo_holder.update(servo_member::left);//状態を変えたいところを引数にとる。引数にとれるのはservo_member::left、servo_member::center、servo_member::rightのみ。openならclose、closeならopenになる。
can_pub_->publish(std::move(doll_holder.send_servo_state()));
```

## エアシリ

table_holder.hppのなか

二値のみ取る。

```c++
solv_sender Solv_sender(0x100);//コンストラクタの引数はCAN_ID 骨董品の電磁弁基板使うなら0x100

can_pub_->publish(std::move(table_holder.update(1)));//状態を変えたいところを引数にとる。0から7まで。onならoff,offならonになる

```

## DC昇降

table_upper.hppのなか

Velモードかposモードか好きな方でやれる。

VEL_TARGETはうまく決めて（昇降の速度になる予定）

多分posモードだと指数関数的な動きをするから昇降には向いてないかも？

posモード用
```c++
DC_upper DC_UPPER(0x100,15.0f);//コンストラクタの引数はCAN_IDとposモードで動かしたい距離。20以上だとシラスが電圧オーバーするかも

can_pub_->publish(std::move(DC_UPPER.update()));//上がってたら下がって、下がってたら上がる
```

velモード用
```c++
DC_upper DC_UPPER(0x100);//コンストラクタの引数はCAN_IDのみ

if(/*昇降を上げるためのトリガー*/){
    can_pub_->publish(std::move(DC_UPPER.rise_by_vel()));
}
else if(/*昇降を下げるためのトリガー*/){
    can_pub_->publish(std::move(DC_UPPER.fall_by_vel()));
}
else{
    can_pub_->publish(std::move(DC_UPPER.stop_vel()));
}
```

# くそな点
・汎用性が皆無<-決められた数値しか送られない方々がいらっしゃる
・ヘッダファイルの名前がくそ
・DC_upperのなかに二つの機能が入っているのはいかがなものか
・実際に動かしてないから動くかわかんない！
