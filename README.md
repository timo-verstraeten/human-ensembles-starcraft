## Example config file:

Should be saved as `config.ini` in `F:\Starcraft`.


Using following configuration, the log and all trials output will be saved under `G:\Project\Test`.

```ini
[TRIAL]
alpha = 0.40
lambda = 0.90
numEpisodes = 200
shapingWeight = 1.0

[EXPERIMENT]
name = Test
numTrials = 5
showLearning = 0

[GENERAL]
outputPath = G:\Project\
enableLogging = 1
saveReplay = 0
```


N.B.: `saveReplay` and `showLearning` are unused/unimplemented in `Config`