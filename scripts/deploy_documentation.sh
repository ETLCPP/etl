#!/bin/bash
set -e

# Set up SSH
mkdir -p ~/.ssh
echo "$ACTIONS_DEPLOY_KEY" > ~/.ssh/deploy_key
chmod 600 ~/.ssh/deploy_key

# Add server to known hosts
ssh-keyscan -p "$SSH_PORT" "$SERVER_ADDRESS" >> ~/.ssh/known_hosts

# Deploy using rsync over SSH
rsync -avz --delete \
  -e "ssh -i ~/.ssh/deploy_key -p $SSH_PORT" \
  hugo/public/ \
  "$SSH_USERNAME@$SERVER_ADDRESS:$SERVER_DESTINATION"