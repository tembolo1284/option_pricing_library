#!/bin/bash
# scripts/deploy_to_aws.sh

# Ensure the script exits on any failure
set -e

# AWS Deployment (placeholder)
echo "Deploying to AWS..."

# Example AWS CLI commands (replace with actual deployment steps)
# aws ecr get-login-password --region us-east-1 | docker login --username AWS --password-stdin <repository_uri>
# docker build -t pricing_server .
# docker tag pricing_server:latest <repository_uri>:latest
# docker push <repository_uri>:latest

echo "Deployment complete."

