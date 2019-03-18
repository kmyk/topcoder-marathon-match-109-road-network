# Topcoder Marathon Match 109: RoadNetwork

https://community.topcoder.com/longcontest/?module=ViewProblemStatement&rd=17427&compid=81910

## 問題

無向グラフ G = (V, E) が与えられる。
辺には重みとして費用 M : V → ℕ と価値 P : V → ℕ が定まっている。
連結であり、自己ループや多重辺はない。

さらに G 上のルートが R 個与えられる。
ただしルートとは頂点の対 (A, B) であって、
それぞれのルート  には価値 P が定まっている。

予算 NM が与えられる。
費用の総和がこれを越えないように辺を選んで部分グラフを作れ。
特に、部分グラフに含まれる辺の価値の総和 P1 とルートの価値の総和 P2 の積 P1 P2 をできるだけ大きくするものがよい。


## 日記

### 2019/03/16 12:00

入力の制約や傾向によって方針が変わりそうな問題に見える。
とりあえず貪欲を書きたい。
辺の費用順、辺のコスパ順、ルートの費用順、ルートのコスパ順、などなど。

### 2019/03/17 03:00

実装した。

### 2019/03/17 21:00

集計をした。辺とルートそれぞれについて (得た点数 / 点数の総和) の平均値。

1.  辺のランダム順
1.  辺のコスパ順
1.  ルートの費用順
1.  ルートの費用順 + 余りで辺のコスパ順
1.  ルートのコスパ順 ((辺評価の総和 + ルート評価) / 辺費用の総和)

``` console
$ COMMITS='... 20673a651715e1226363c8910012077444081827'
$ for commit in $COMMITS ; do git checkout ${commit} RoadNetwork.cpp ; cxx main.cpp -o bin/$commit ; done
$ for commit in $COMMITS ; do for i in $(seq 1000) ; do java -jar tester.jar -exec ./bin/$commit -seed ${i}000 -novis -json > log/${commit}.${i}.json ; done ; done
$ for commit in $COMMITS ; do for i in $(seq 2000) ; do cat log/${commit}.${i}.json ; done | jq --slurp '[ ([ .[] | .details.edgeScore / ([ .edges[] | .[3] ] | add) ] | add / length), ([ .[] | .details.routeScore / ([ .routes[] | .[2] ] | add) ] | add / length) ]' ; done
[
  0.12896243752421782,
  0.0004693963686985877
]
[
  0.21155468186470044,
  0.0006930874832891006
]
[
  0.12095579870967947,
  0.5571152809237752
]
[
  0.13422784723756195,
  0.5571152809237752
]
[
  0.1436605393105044,
  0.6228213963223839
]
```

辺のコスパ順でも 0.2 程度と、意外と辺の重みが使えないことが分かる。
これは単純なナップサック問題なので、ほぼ上限値と言ってよいだろう。

ルートの上限は未知数。

### 2019/03/18 00:00

ひとつめに使うルートを総当たりする解を書いた。

### 2019/03/18 01:00

フローがありそうな気がしてきた。
無向なのでフローではないけど、雰囲気は近いので見ておくことはあり。
答えを決め打ちしての最小費用流とか。
難しそう？

適当な根を決めて有向グラフ化するのはどうか。
これはルートの取り扱いを面倒にするだけに見える。

ルート間の相互関係を取り扱えていない。
ルートの構築に寄与するような辺にボーナスを与え、その順に取る貪欲はどうか。
複数のルートの共通部分ルートとなるようなルートを作り、これをルート集合に足すのはどうか。

そろそろ焼きなましやビームサーチ方向への展開を初める必要があるのではないか。

### 2019/03/18 02:50

とりあえず提出。
ルートのコスパ順 ((辺評価の総和 + ルート評価) / 辺費用の総和) のもの `20673a651715e1226363c8910012077444081827` を使った。
ひとつめのルートを総当たりする解はまだ評価が済んでいないのとバグが残ってそうだったため。

<details>
<summary>順位表</summary>

| Handle           | Score      | Rank  | Last Submission Time  | Language  | Example Tests  | Submissions  |
|------------------|------------|-------|-----------------------|-----------|----------------|--------------|
| wleite           | 981684.78  | 1     | 03.17.2019 13:35:50   | Java      | 6              | 6            |
| EvbCFfp1XB       | 932706.34  | 2     | 03.17.2019 09:24:29   | Java      | 2              | 2            |
| mugurelionut     | 921880.78  | 3     | 03.17.2019 11:33:56   | C++       | 11             | 4            |
| yowa             | 919719.61  | 4     | 03.17.2019 11:18:16   | C++       | 3              | 3            |
| iwashi31         | 910250.86  | 5     | 03.17.2019 10:24:25   | C++       | 9              | 6            |
| nika             | 904174.56  | 6     | 03.16.2019 16:19:35   | C++       | 2              | 1            |
| kotamanegi       | 892005.72  | 7     | 03.17.2019 04:41:56   | C++       | 4              | 3            |
| iehn             | 850766.30  | 8     | 03.17.2019 08:44:16   | C++       | 15             | 3            |
| gasin            | 837532.04  | 9     | 03.16.2019 22:09:25   | C++       | 2              | 2            |
| kimiyuki         | 824296.54  | 10    | 03.17.2019 13:42:36   | C++       | 1              | 1            |
| kosakkun         | 819520.83  | 11    | 03.17.2019 11:34:26   | C++       | 4              | 4            |
| HrKdr            | 790451.74  | 12    | 03.17.2019 13:19:51   | C++       | 2              | 2            |
| tanzaku          | 788267.69  | 13    | 03.17.2019 06:35:55   | C++       | 2              | 2            |
| windhunterSB     | 772374.70  | 14    | 03.17.2019 11:25:38   | C++       | 3              | 3            |
| yuya178          | 759954.53  | 15    | 03.17.2019 02:08:29   | Java      | 12             | 4            |
| my316g           | 709340.16  | 16    | 03.17.2019 10:53:07   | C++       | 1              | 1            |
| vi002            | 684057.50  | 17    | 03.17.2019 11:41:49   | C++       | 2              | 2            |
| ts__             | 655237.71  | 18    | 03.17.2019 12:32:38   | C++       | 2              | 2            |
| nel215           | 651648.95  | 19    | 03.17.2019 04:11:07   | C++       | 1              | 1            |
| seess_yshysj     | 568583.98  | 20    | 03.16.2019 21:23:06   | C++       | 8              | 3            |
| kishore_g84      | 523491.10  | 21    | 03.17.2019 02:23:53   | Java      | 1              | 2            |
| ebicochineal     | 477448.36  | 22    | 03.17.2019 05:51:20   | C++       | 1              | 3            |
| bduvenhage       | 459662.13  | 23    | 03.17.2019 02:52:33   | C++       | 2              | 2            |
| AmAtUrECoDeR     | 422320.57  | 24    | 03.17.2019 10:00:49   | Java      | 6              | 2            |
| mmammel          | 253987.14  | 25    | 03.17.2019 00:12:41   | C++       | 2              | 1            |
| onigiri          | 145261.74  | 26    | 03.17.2019 10:11:38   | Java      | 0              | 1            |
| abdullahkool768  | 2478.86    | 27    | 03.16.2019 04:18:04   | C++       | 2              | 1            |
| the_lizard       | 945.31     | 28    | 03.17.2019 08:57:38   | C++       | 1              | 1            |
| adist98          | 161.33     | 29    | 03.17.2019 07:10:24   | C++       | 9              | 3            |
| r2d1             |            |       |                       |           | 1              | 0            |
| sumoru           |            |       |                       |           | 8              | 0            |
| usto             |            |       |                       |           | 1              | 0            |
| yarrr            |            |       |                       |           | 1              | 0            |

</details>

1位の wleite さんと比べると点数を 981684.78 / 824296.54 = 1.1909364 倍にできることが分かる。
現状の点数は 0.14366054 * 0.62282140 であるが、辺とルートをそれぞれ √2 倍すればよいと考えると 0.15737224 * 0.68226666 が目標となる。

まだ単純な貪欲解なので10位でも悲観することはなさそう。

### 2019/03/18 03:40

forum を読むのを忘れていたので読んだ。

visualizer の更新があったので更新した。
branch [official-visualizer](https://github.com/kmyk/topcoder-marathon-match-109-road-network/tree/official-visualizer) を生やして merge してあげたら手元での更新との合成がとても楽だった。

### 2019/03/18 04:10

ひとつめのルートを総当たり `42d69838a9dac2320aabaab8afeebb6efcebbd1c` を提出。バグ修正済み。
でも9位。これはちょっと焦る。
赤維持どころか [TCO Japan regional](https://tco19.topcoder.com/regional-events/tco19-regional-event-japan) に行くにも点数が足りなさそう。

<details>
<summary>順位表</summary>

| Handle          |     Score |   Rank | Last Submission Time   | Language   |   Example Tests |   Submissions |
|-----------------|-----------|--------|------------------------|------------|-----------------|---------------|
| wleite          | 981637    |      1 | 03.17.2019 13:35:50    | Java       |               6 |             6 |
| EvbCFfp1XB      | 932661    |      2 | 03.17.2019 09:24:29    | Java       |               2 |             2 |
| mugurelionut    | 921833    |      3 | 03.17.2019 11:33:56    | C++        |              13 |             4 |
| yowa            | 919671    |      4 | 03.17.2019 11:18:16    | C++        |               3 |             3 |
| iwashi31        | 910202    |      5 | 03.17.2019 10:24:25    | C++        |               9 |             6 |
| nika            | 904129    |      6 | 03.16.2019 16:19:35    | C++        |               2 |             1 |
| kotamanegi      | 891958    |      7 | 03.17.2019 04:41:56    | C++        |               4 |             3 |
| tanzaku         | 887557    |      8 | 03.17.2019 14:07:24    | C++        |               3 |             3 |
| kimiyuki        | 887552    |      9 | 03.17.2019 15:49:30    | C++        |               5 |             2 |
| iehn            | 850720    |     10 | 03.17.2019 08:44:16    | C++        |              15 |             3 |
| gasin           | 837488    |     11 | 03.16.2019 22:09:25    | C++        |               2 |             2 |
| kosakkun        | 819480    |     12 | 03.17.2019 11:34:26    | C++        |               4 |             4 |
| HrKdr           | 791097    |     13 | 03.17.2019 15:30:35    | C++        |               3 |             3 |
| windhunterSB    | 772335    |     14 | 03.17.2019 11:25:38    | C++        |               3 |             3 |
| yuya178         | 759916    |     15 | 03.17.2019 02:08:29    | Java       |              12 |             4 |
| my316g          | 709301    |     16 | 03.17.2019 10:53:07    | C++        |               1 |             1 |
| vi002           | 699248    |     17 | 03.17.2019 14:46:17    | C++        |               3 |             3 |
| ts__            | 655205    |     18 | 03.17.2019 12:32:38    | C++        |               2 |             2 |
| nel215          | 651614    |     19 | 03.17.2019 04:11:07    | C++        |               1 |             1 |
| seess_yshysj    | 568561    |     20 | 03.16.2019 21:23:06    | C++        |               8 |             3 |
| kishore_g84     | 523461    |     21 | 03.17.2019 02:23:53    | Java       |               1 |             2 |
| ebicochineal    | 477425    |     22 | 03.17.2019 05:51:20    | C++        |               1 |             3 |
| bduvenhage      | 459641    |     23 | 03.17.2019 02:52:33    | C++        |               2 |             2 |
| AmAtUrECoDeR    | 422303    |     24 | 03.17.2019 10:00:49    | Java       |               6 |             2 |
| mmammel         | 253978    |     25 | 03.17.2019 00:12:41    | C++        |               2 |             1 |
| onigiri         | 145254    |     26 | 03.17.2019 10:11:38    | Java       |               0 |             1 |
| abdullahkool768 |   2478.86 |     27 | 03.16.2019 04:18:04    | C++        |               2 |             1 |
| the_lizard      |    944.96 |     28 | 03.17.2019 08:57:38    | C++        |               1 |             1 |
| adist98         |    161.32 |     29 | 03.17.2019 07:10:24    | C++        |               9 |             3 |
| r2d1            |           |        |                        |            |               1 |             0 |
| sumoru          |           |        |                        |            |               8 |             0 |
| usto            |           |        |                        |            |               1 |             0 |
| yarrr           |           |        |                        |            |               1 |             0 |

</details>

ところで `oj submit` の `--full-submission` が壊れた？
さっきは提出に成功したのに今は → デグレ <https://github.com/kmyk/online-judge-tools/issues/393>

ちなみに点数はちゃんと上がってる。

``` console
$ for commit in 42d69838a9dac2320aabaab8afeebb6efcebbd1c ; do for i in $(seq 2000) ; do cat log/${commit}/${i}.json ; done | jq --slurp '[ ([ .[] | .details.edgeScore / ([ .edges[] | .[3] ] | add) ] | add / length), ([ .[] | .details.routeScore / ([ .routes[] | .[2] ] | add) ] | add / length) ]' ; done
[
  0.1466205469750911,
  0.6530273896351924
]
```

### 2019/03/18 06:30

使用する辺を追加や交換する単純な焼きなましを書くべきな気がしてきた。

解法本体と visualizer や Makefile などを別の git の commit tree で管理したい気持ちがあるが、実はできるらしい: [Two git repositories in one directory? - Stack Overflow](https://stackoverflow.com/questions/436125/two-git-repositories-in-one-directory), [Two different Git repo in same directory - Stack Overflow](https://stackoverflow.com/questions/8778796/two-different-git-repo-in-same-directory)。
しかし微妙に面倒そうでもある。
現状では `documents` ディレクトリ内に [documents](https://github.com/kmyk/topcoder-marathon-match-109-road-network/tree/documents) branch を clone してをそこを別管理としているが、 `tools` のようなディレクトリを切って同様のことをする方が楽そうである。


### 2019/03/18 08:20

使用するルートの集合を固定されたとき、その充足可能性判定はできるか？ あるいはその費用の最小値は求まるか？

グラフは平面グラフに限られているように見えるが、その事実は使えないか？

ルートとしてありそうなパスをいくつか列挙し、これの部分集合を焼きなますのはどうか？

### 2019/03/18 09:20

visualizer を読んだ。
平面グラフを作ってる。

1.  頂点を座標付きでランダムに生成する
1.  全体が連結になるまで辺を次のように足す:
    -   点 a をランダムに選ぶ
    -   その最も近くてかつ平面描画であることを崩さない点 b を選ぶ
    -   費用 m は距離 d(a, b) をそのまま使う
    -   価値 p は距離 d(a, b) を係数としてランダムに選ぶ
1.  ルートをランダムに生成する
    -   価値 p は距離 d(a, b) をそのまま使う
1.  予算 NM はルートの価値の総和を係数としてランダムに選ぶ

### 2019/03/18 09:50

閉路をひとつ選んでその中の辺の使用/不使用を反転させるのはどうか？
特に長さ 3 の閉路ですべてが使用でもすべてが不使用でもないものを考えると、これは近傍の内径をかなり小さくできそう。

### 2019/03/18 10:40

画像を出力できるようにした

<details>
<summary>画像: 貪欲, 辺コスパ順</summary>

[6c0b6a595739e994b57aeb153393bd0a933445f9](https://github.com/kmyk/topcoder-marathon-match-109-road-network/commit/6c0b6a595739e994b57aeb153393bd0a933445f9)

![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/6c0b6a595739e994b57aeb153393bd0a933445f9.1.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/6c0b6a595739e994b57aeb153393bd0a933445f9.2.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/6c0b6a595739e994b57aeb153393bd0a933445f9.3.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/6c0b6a595739e994b57aeb153393bd0a933445f9.4.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/6c0b6a595739e994b57aeb153393bd0a933445f9.5.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/6c0b6a595739e994b57aeb153393bd0a933445f9.6.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/6c0b6a595739e994b57aeb153393bd0a933445f9.7.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/6c0b6a595739e994b57aeb153393bd0a933445f9.8.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/6c0b6a595739e994b57aeb153393bd0a933445f9.9.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/6c0b6a595739e994b57aeb153393bd0a933445f9.10.png)

</details>

<details>
<summary>画像: 貪欲, ルートコスパ順</summary>

[20673a651715e1226363c8910012077444081827](https://github.com/kmyk/topcoder-marathon-match-109-road-network/commit/20673a651715e1226363c8910012077444081827)

![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/20673a651715e1226363c8910012077444081827.1.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/20673a651715e1226363c8910012077444081827.2.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/20673a651715e1226363c8910012077444081827.3.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/20673a651715e1226363c8910012077444081827.4.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/20673a651715e1226363c8910012077444081827.5.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/20673a651715e1226363c8910012077444081827.6.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/20673a651715e1226363c8910012077444081827.7.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/20673a651715e1226363c8910012077444081827.8.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/20673a651715e1226363c8910012077444081827.9.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/20673a651715e1226363c8910012077444081827.10.png)

</details>

<details>
<summary>画像: 貪欲, ルートコスパ順 + 1手目総当たり</summary>

[42d69838a9dac2320aabaab8afeebb6efcebbd1c](https://github.com/kmyk/topcoder-marathon-match-109-road-network/commit/42d69838a9dac2320aabaab8afeebb6efcebbd1c)

![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/42d69838a9dac2320aabaab8afeebb6efcebbd1c.1.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/42d69838a9dac2320aabaab8afeebb6efcebbd1c.2.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/42d69838a9dac2320aabaab8afeebb6efcebbd1c.3.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/42d69838a9dac2320aabaab8afeebb6efcebbd1c.4.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/42d69838a9dac2320aabaab8afeebb6efcebbd1c.5.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/42d69838a9dac2320aabaab8afeebb6efcebbd1c.6.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/42d69838a9dac2320aabaab8afeebb6efcebbd1c.7.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/42d69838a9dac2320aabaab8afeebb6efcebbd1c.8.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/42d69838a9dac2320aabaab8afeebb6efcebbd1c.9.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/42d69838a9dac2320aabaab8afeebb6efcebbd1c.10.png)

</details>

<details>
<summary>画像: 焼きなまし, 雑</summary>

[24418d448d1734bc19dc2055b3791f28a682972c](https://github.com/kmyk/topcoder-marathon-match-109-road-network/commit/24418d448d1734bc19dc2055b3791f28a682972c)

![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/24418d448d1734bc19dc2055b3791f28a682972c.1.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/24418d448d1734bc19dc2055b3791f28a682972c.2.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/24418d448d1734bc19dc2055b3791f28a682972c.3.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/24418d448d1734bc19dc2055b3791f28a682972c.4.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/24418d448d1734bc19dc2055b3791f28a682972c.5.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/24418d448d1734bc19dc2055b3791f28a682972c.6.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/24418d448d1734bc19dc2055b3791f28a682972c.7.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/24418d448d1734bc19dc2055b3791f28a682972c.8.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/24418d448d1734bc19dc2055b3791f28a682972c.9.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/24418d448d1734bc19dc2055b3791f28a682972c.10.png)

</details>
