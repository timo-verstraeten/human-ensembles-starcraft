library(ggplot2)

# Define settings
alpha = 0.4
lambda = 0.95
base_folder = ""
# There have to exist some folders with the EXACT name as in the "settings"
settings = c("experiment1", "experiment2")
trials_per_setting = 10
num_episodes = 150

# Data collection and aggregation
dta = data.frame()
for(s in settings){
  for(j in seq(0, trials_per_setting-1)){
    trial_file = paste0(base_folder, s, "/trial", j, "_out.txt", sep="")
    trial_data = read.csv(trial_file, header=F, col.names=c("episode","reward", "steps"))[1:num_episodes, ]
    trial_data$episode = c(1:num_episodes)
    trial_data$setting = s
    trial_data$trial = j
    dta = rbind(dta, trial_data)
  }
}
dta$setting = factor(dta$setting, levels=settings)
remove(trial_data, s, j)

# Plotting fun
plt = function(y_string, show_confidence=TRUE){
  ggplot(aes_string(x="episode", y=y_string, color="setting"), data=dta) + 
    geom_smooth(size=1.1, se=show_confidence) + 
    ggtitle(paste0('Smoothed ',y_string, ', with alpha=', alpha,' and lambda=',lambda))
}

plt("reward", TRUE)
plt("steps", FALSE)
