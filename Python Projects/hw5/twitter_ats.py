"""
File:    twiiter_ats.py
Author:  Zain Latif
Date:    10/15/2024
Section: 44
E-mail:  zlatif1@umbc.edu
Description:
  makes a tweet
"""
def twitter_ats(the_tweet):
    usernames = []
    hashtags = []

    
    words = the_tweet.split()

    for word in words:
        if len(word) > 1 and word[0] == '@': 
            username = word[1:]  
            found = False
            for existing_user in usernames:
                if existing_user == username:
                    found = True
            if not found:  
                usernames.append(username)

        if len(word) > 1 and word[0] == '#':  
            hashtag = word[1:]  
            found = False
            for existing_tag in hashtags:
                if existing_tag == hashtag:
                    found = True
            if not found:  
                hashtags.append(hashtag)

    return [usernames, hashtags]


if __name__ == "__main__":
    tweet = input("What do you want to tweet? ")
    while tweet.lower() != 'quit':  
        users, tags = twitter_ats(tweet)
        print("The users were:", ', '.join(users))
        print("The hash-tags are:", ', '.join(tags))
        tweet = input("What do you want to tweet? ")  
